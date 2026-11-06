// =====================================================================
//  Nasadara mission init -- vanilla mission + Dog Tamer v2 
//
//  Dog taming: stand within ~3 m of a wild guard dog -> it tames to you.
//  Tamed dogs follow you (70 m leash) and bite nearby infected.
// =====================================================================

class DogTamerConstants
{
    static const float TAME_RADIUS    = 3.0;
    static const float FOLLOW_RADIUS  = 20.0;
    static const float LEASH_RADIUS   = 70.0;
    static const float GUARD_RADIUS   = 12.0;
    static const float COMMAND_RADIUS = 15.0;
    static const float BITE_RANGE     = 2.8;
    static const float BITE_DAMAGE    = 3.0;
    static const float FOLLOW_OFFSET  = 1.6;
    static const float FOLLOW_LERP    = 0.55;
    static const int   TICK_MS        = 300;
    static const int   SCAN_EVERY     = 3;

    // dog states
    static const int STATE_FOLLOW = 0;
    static const int STATE_STAY   = 1;
    static const int STATE_ATTACK = 2;
}

class TamedDog
{
    EntityAI   m_Dog;
    PlayerBase m_Owner;
    string     m_OwnerName;
    string     m_Breed;
    int        m_State;

    void TamedDog(EntityAI dog, PlayerBase owner, string breed)
    {
        m_Dog = dog;
        m_Owner = owner;
        m_Breed = breed;
        m_State = DogTamerConstants.STATE_FOLLOW;
        if (owner)
            m_OwnerName = owner.GetName();
        else
            m_OwnerName = "";
    }

    bool OwnerAlive()
    {
        return m_Owner != null && m_Owner.IsAlive();
    }
}

ref DogTamer g_Tamer;

class DogTamer
{
    ref array<TamedDog> m_Tamed;
    int                 m_TickCount;

    void DogTamer()
    {
        m_Tamed = new array<TamedDog>();
        m_TickCount = 0;
    }

    // ---- identification helpers -----------------------------------
    static string BreedOf(EntityAI e)
    {
        if (Animal_CanisFamiliaris_Dobermann_ColorBase.Cast(e))
            return "Dobermann";
        if (Animal_CanisFamiliaris_GermanShepherd_ColorBase.Cast(e))
            return "GermanShepherd";
        return "";
    }

    static int CurrentEmote(PlayerBase player)
    {
        EmoteManager em = player.GetEmoteManager();
        if (!em)
            return -1;
        if (!em.IsEmotePlaying())
            return -1;
        EmoteLauncher l = em.GetEmoteLauncher();
        if (!l)
            return -1;
        return l.GetID();
    }

    static string HeldType(PlayerBase player)
    {
        ItemBase it = player.GetItemInHands();
        if (!it)
            return "";
        return it.GetType();
    }

    static bool IsAnimalMeat(string cn)
    {
        if (cn == "ChickenBreastMeat")      return true;
        if (cn == "CarpFilletMeat")         return true;
        if (cn == "CatfishFilletMeat")      return true;
        if (cn == "MackerelFilletMeat")     return true;
        if (cn == "Carp")                   return true;
        if (cn == "Catfish")                return true;
        if (cn == "Mackerel")               return true;
        if (cn == "Sardines")               return true;
        if (cn == "Shrimp")                 return true;
        if (cn == "RoundGoby")              return true;
        if (cn == "Bitterlings")            return true;
        if (cn == "SteelheadTrout")         return true;
        if (cn == "WalleyePollock")         return true;
        if (cn == "SteelheadTroutFilletMeat") return true;
        if (cn == "WalleyePollockFilletMeat") return true;
        if (cn == "BearSteakMeat")          return true;
        if (cn == "BoarSteakMeat")          return true;
        if (cn == "CowSteakMeat")           return true;
        if (cn == "DeerSteakMeat")          return true;
        if (cn == "DogSteakMeat")           return true;
        if (cn == "FoxSteakMeat")           return true;
        if (cn == "GoatSteakMeat")          return true;
        if (cn == "MouflonSteakMeat")       return true;
        if (cn == "PigSteakMeat")           return true;
        if (cn == "RabbitLegMeat")          return true;
        if (cn == "ReindeerSteakMeat")      return true;
        if (cn == "SheepSteakMeat")         return true;
        if (cn == "WolfSteakMeat")          return true;
        if (cn == "LizardMeat")             return true;
        return false;
    }

    static bool IsHumanMeat(string cn)
    {
        return cn == "HumanSteakMeat";
    }

    // ---- main tick ------------------------------------------------
    void Tick()
    {
        if (!g_Game || !g_Game.IsDedicatedServer())
            return;

        m_TickCount++;
        bool doScan = (m_TickCount % DogTamerConstants.SCAN_EVERY == 0);

        for (int i = 0; i < m_Tamed.Count(); i++)
        {
            ref TamedDog td = m_Tamed[i];
            if (!td.m_Dog || !td.m_Dog.IsAlive())
                continue;
            Follow(td);
            if (doScan)
                Guard(td);
        }

        HandleEmotes();
    }

    // ---- emote commands -------------------------------------------
    void HandleEmotes()
    {
        ref array<Man> players = new array<Man>();
        g_Game.GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (!player || !player.IsAlive())
                continue;

            int emote = CurrentEmote(player);
            if (emote < 0)
                continue;

            vector pp = player.GetPosition();
            ref TamedDog own = OwnedDog(player);

            // TAME: GREETING + animal meat + no current dog + wild dog nearby
            if (emote == EmoteConstants.ID_EMOTE_GREETING)
            {
                if (!own && IsAnimalMeat(HeldType(player)))
                {
                    ref EntityAI dog = NearestWildDog(pp, DogTamerConstants.TAME_RADIUS);
                    if (dog)
                    {
                        NeutralizeAI(dog);
                        ref TamedDog td = new TamedDog(dog, player, BreedOf(dog));
                        m_Tamed.InsertAt(td, m_Tamed.Count());
                        Print(string.Format("[DogTamer] %1 tamed a %2 (emote+meat)", player.GetName(), td.m_Breed));
                    }
                }
                continue;
            }

            // COMMANDS on the player's own dog (must be within command radius)
            if (!own)
                continue;
            float ownDist = Dist(pp, own.m_Dog.GetPosition());
            if (ownDist > DogTamerConstants.COMMAND_RADIUS)
                continue;

            if (emote == EmoteConstants.ID_EMOTE_COME)
            {
                own.m_State = DogTamerConstants.STATE_FOLLOW;
            }
            else if (emote == EmoteConstants.ID_EMOTE_SILENT)
            {
                own.m_State = DogTamerConstants.STATE_STAY;
            }
            else if (emote == EmoteConstants.ID_EMOTE_TAUNT)
            {
                own.m_State = DogTamerConstants.STATE_ATTACK;
            }
            else if (emote == EmoteConstants.ID_EMOTE_SURRENDER && IsHumanMeat(HeldType(player)))
            {
                RemoveTamed(own);
                Print(string.Format("[DogTamer] %1 untamed their dog (human meat)", player.GetName()));
            }
        }
    }

    // ---- follow behavior ------------------------------------------
    void Follow(ref TamedDog td)
    {
        if (!td.OwnerAlive())
            return;
        if (td.m_State != DogTamerConstants.STATE_FOLLOW)
            return;   // STAY / ATTACK: Guard() or no movement

        vector op = td.m_Owner.GetPosition();
        vector dp = td.m_Dog.GetPosition();
        float dist = Dist(dp, op);

        if (dist <= DogTamerConstants.FOLLOW_RADIUS)
        {
            vector dir = td.m_Owner.GetDirection();
            vector behind = Vector(
                op[0] - dir[0] * DogTamerConstants.FOLLOW_OFFSET,
                dp[1],
                op[2] - dir[2] * DogTamerConstants.FOLLOW_OFFSET);
            if (dist > 0.5)
                MoveTowards(td.m_Dog, dp, behind, DogTamerConstants.FOLLOW_LERP);
            td.m_Dog.SetOrientation(td.m_Owner.GetOrientation());
        }
        else if (dist <= DogTamerConstants.LEASH_RADIUS)
        {
            MoveTowards(td.m_Dog, dp, op, 0.08);
        }
    }

    void MoveTowards(EntityAI ent, vector from, vector to, float t)
    {
        vector np = Vector(
            from[0] + (to[0] - from[0]) * t,
            from[1] + (to[1] - from[1]) * t,
            from[2] + (to[2] - from[2]) * t);
        ent.SetPosition(np);
    }

    float Dist(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dy = a[1] - b[1];
        float dz = a[2] - b[2];
        return Math.Sqrt(dx * dx + dy * dy + dz * dz);
    }

    // ---- guard / attack -------------------------------------------
    void Guard(ref TamedDog td)
    {
        if (!td.OwnerAlive())
            return;

        vector dogPos = td.m_Dog.GetPosition();
        bool wantsAttack = (td.m_State == DogTamerConstants.STATE_ATTACK);

        if (!wantsAttack)
        {
            float ownerDist = Dist(dogPos, td.m_Owner.GetPosition());
            if (ownerDist > DogTamerConstants.GUARD_RADIUS)
                return;
        }

        ref EntityAI target = NearestInfected(dogPos, DogTamerConstants.GUARD_RADIUS);
        if (!target)
        {
            if (wantsAttack)
                td.m_State = DogTamerConstants.STATE_FOLLOW;
            return;
        }

        float tdist = Dist(dogPos, target.GetPosition());
        if (tdist <= DogTamerConstants.BITE_RANGE)
            Bite(target, DogTamerConstants.BITE_DAMAGE);
        else
            MoveTowards(td.m_Dog, dogPos, target.GetPosition(), 0.5);
    }

    void Bite(EntityAI target, float dmg)
    {
        float h = target.GetHealth();
        if (h > 0.0)
            target.SetHealth(h - dmg);
    }

    ref EntityAI NearestInfected(vector center, float radius)
    {
        vector min = Vector(center[0] - radius, center[1] - radius, center[2] - radius);
        vector max = Vector(center[0] + radius, center[1] + radius, center[2] + radius);
        ref array<EntityAI> list = new array<EntityAI>();
        DayZPlayerUtils.SceneGetEntitiesInBox(min, max, list);

        ref EntityAI best = null;
        float bestDist = radius;
        for (int i = 0; i < list.Count(); i++)
        {
            EntityAI e = list[i];
            if (!e || !e.IsAlive())
                continue;
            if (!ZombieBase.Cast(e))
                continue;
            float d = Dist(center, e.GetPosition());
            if (d < bestDist)
            {
                bestDist = d;
                best = e;
            }
        }
        return best;
    }

    // ---- taming / removal -----------------------------------------
    ref EntityAI NearestWildDog(vector center, float radius)
    {
        vector min = Vector(center[0] - radius, center[1] - radius, center[2] - radius);
        vector max = Vector(center[0] + radius, center[1] + radius, center[2] + radius);
        ref array<EntityAI> list = new array<EntityAI>();
        DayZPlayerUtils.SceneGetEntitiesInBox(min, max, list);

        ref EntityAI best = null;
        float bestDist = radius;
        for (int i = 0; i < list.Count(); i++)
        {
            EntityAI e = list[i];
            if (!e || !e.IsAlive())
                continue;
            if (BreedOf(e) == "")
                continue;
            if (IsAlreadyTamed(e))
                continue;
            float d = Dist(center, e.GetPosition());
            if (d < bestDist)
            {
                bestDist = d;
                best = e;
            }
        }
        return best;
    }

    void NeutralizeAI(EntityAI dog)
    {
        ref DayZAnimal animal = DayZAnimal.Cast(dog);
        if (animal)
            animal.DestroyAIAgent();
    }

    ref TamedDog OwnedDog(PlayerBase player)
    {
        for (int i = 0; i < m_Tamed.Count(); i++)
        {
            ref TamedDog td = m_Tamed[i];
            if (td.m_Owner == player)
                return td;
        }
        return null;
    }

    bool IsAlreadyTamed(EntityAI dog)
    {
        for (int i = 0; i < m_Tamed.Count(); i++)
        {
            ref TamedDog td = m_Tamed[i];
            if (td.m_Dog == dog)
                return true;
        }
        return false;
    }

    void RemoveTamed(ref TamedDog target)
    {
        ref array<TamedDog> keep = new array<TamedDog>();
        for (int i = 0; i < m_Tamed.Count(); i++)
        {
            ref TamedDog td = m_Tamed[i];
            if (td != target)
                keep.InsertAt(td, keep.Count());
        }
        m_Tamed = keep;
    }
}

// ---- global tick ----------------------------------------------------
void TamerTick()
{
    if (g_Tamer)
        g_Tamer.Tick();
}

// ---- boundary marker (the merge cuts everything before this) --------


// =====================================================================
//  Vanilla mission (unchanged, plus the OnInit hook above)
// =====================================================================
void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 4, reset_day = 30;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.45, 0.65 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		player.RemoveAllItems();
		
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		string bodyArray[] = { "KameezShirt_Beige", "KameezShirt_Black", "KameezShirt_BlueFlowers", "KameezShirt_Brown", "KameezShirt_Grey", "KameezShirt_NavyBlue", "KameezShirt_Olive", "KameezShirt_Orange", "KameezShirt_Pink", "KameezShirt_Purple", "KameezShirt_Red", "KameezShirt_SkyBlue", "KameezShirt_White", "KameezShirt_Yellow" };
		int rndBodyIndex = Math.RandomInt( 0, 14 );
		itemEnt = player.GetInventory().CreateInInventory( bodyArray[rndBodyIndex] );
		SetRandomHealth( itemEnt );
		
		string legsArray[] = { "MensShalwarPants_Beige", "MensShalwarPants_Black", "MensShalwarPants_Brown", "MensShalwarPants_Grey", "MensShalwarPants_NavyBlue", "MensShalwarPants_SkyBlue", "MensShalwarPants_White", "WomensShalwarPants_NavyBlue", "WomensShalwarPants_Olive", "WomensShalwarPants_Orange", "WomensShalwarPants_Pink", "WomensShalwarPants_Purple", "WomensShalwarPants_Red" };
		int rndLegsIndex = Math.RandomInt( 0, 13 );
		itemEnt = player.GetInventory().CreateInInventory( legsArray[rndLegsIndex] );
		SetRandomHealth( itemEnt );
		
		string feetArray[] = { "SandalsShoes_Black", "SandalsShoes_BlueNavy", "SandalsShoes_Brown", "SandalsShoes_Grey", "TraditionalSandalShoes_Black", "TraditionalSandalShoes_Brown", "TraditionalSandalShoes_LightBrown", "TraditionalSandalShoes_NavyBlue" };
		int rndFeetIndex = Math.RandomInt( 0, 8 );
		itemEnt = player.GetInventory().CreateInInventory( feetArray[rndFeetIndex] );
		SetRandomHealth( itemEnt );
		
		string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
		int rndChemIndex = Math.RandomInt( 0, 4 );
		itemEnt = player.GetInventory().CreateInInventory( chemlightArray[rndChemIndex] );
		player.SetQuickBarEntityShortcut(itemEnt, 1);
		SetRandomHealth( itemEnt );

		string sodaArray[] = { "SodaCan_Pipsi", "SodaCan_Spite", "SodaCan_Cola", "SodaCan_Fronta" };
		int rndSodaIndex = Math.RandomInt( 0, 4 );
		itemEnt = player.GetInventory().CreateInInventory( sodaArray[rndSodaIndex] );
		SetRandomHealth( itemEnt );
		
		itemEnt = player.GetInventory().CreateInInventory( "Figs" );
		itemEnt = player.GetInventory().CreateInInventory( "BandageDressing" );
		player.SetQuickBarEntityShortcut(itemEnt, 2);
		
		player.GetStatWater().Set( 1000 );
		player.GetStatEnergy().Set( 800 );
	}

	// ---- DogTamer v1 hook (merged from init.c_tameable) ----
	override void OnInit()
	{
		super.OnInit();   // base MissionServer init (PlayerSpawnHandler, CfgGameplayHandler, etc.)
		g_Tamer = new DogTamer();
		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TamerTick,
			DogTamerConstants.TICK_MS, true);
		Print("[DogTamer] taming mechanic active (nasdara)");
	}

};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}