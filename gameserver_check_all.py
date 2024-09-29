import json
import os
import requests
import re
import logging

# Ensure the API key is set
API_KEY = os.getenv("NITRADO_TOKEN")
if not API_KEY:
    print("Error: NITRADO_TOKEN environment variable is not set.")
    exit(1)

def fetch_gameserver_details(service_id, api_key):
    """Fetch gameserver details from the Nitrado API."""
    response = requests.get(
        'https://api.nitrado.net/services/{}/gameservers'.format(service_id),
        headers={'Authorization': 'Bearer {}'.format(api_key)}
    )
    if response.ok:
        return response.json().get("data", {}).get("gameserver", {})
    else:
        logging.warning("Error fetching gameserver details for service ID {}: {} - {}".format(service_id, response.status_code, response.text))
        return None

def format_server_name(gameserver):
    """Format the server name, removing unwanted characters."""
    server_name = gameserver.get("query", {}).get("server_name", "Server Name Not Available")
    server_name = re.sub(r'[^a-zA-Z0-9 ]', '', server_name)  # Allow letters, numbers, and spaces
    return server_name or "Server Name Not Available"

def get_status_message(status):
    """Get a formatted message for the gameserver status."""
    status_messages = {
        "started": "🔵 **The Server is up and running.**",
        "stopped": "🔴 **The Server is stopped.**",
        "stopping": "🟡 **The Server is currently stopping.**",
        "restarting": "🔄 **The Server is currently restarting. This can take some minutes.**",
        "suspended": "⚠️ **The server is suspended and needs to be reactivated on the website.**",
        "guardian_locked": "🔒 **Your services are guardian protected; you are currently outside of the allowed times.**",
        "gs_installation": "⚙️ **The server is currently performing a game switching action.**",
        "backup_restore": "🔄 **A backup will be restored now.**",
        "backup_creation": "💾 **A new backup will be created now.**",
        "chunkfix": "🗺️ **The Server is running a Minecraft chunkfix.**",
        "overviewmap_render": "🗺️ **The Server is running a Minecraft Overview Map rendering.**",
    }
    return status_messages.get(status, "❓ **Unknown status.**")

def generate_markdown(services, api_key):
    """Generate enhanced Markdown output for the gameserver details."""
    markdown_output = "# 🎮 **Gameserver Details**\n\n"
    markdown_output += "Here are the details for your gameservers hosted on Nitrado. Enjoy the game! 🎉\n\n"

    for service in services:
        service_id = service.get("id")
        gameserver = fetch_gameserver_details(service_id, api_key)

        if gameserver:
            server_name = format_server_name(gameserver)
            markdown_output += "## 🖥️ **{}**\n\n".format(server_name)

            markdown_output += "| **Property**         | **Value**                   |\n"
            markdown_output += "|----------------------|------------------------------|\n"

            player_count = gameserver.get("query", {}).get("player_current", 0)
            max_slots = gameserver.get("slots", 0)

            properties = {
                "Status": get_status_message(gameserver.get('status', 'Unknown')),
                "Player Count": "👥 **{}/{}**".format(player_count, max_slots),
                "Last Update": "🕒 **{}**".format(gameserver.get('game_specific', {}).get('last_update', 'None')),
                "Comment": "💬 **{}**".format(service.get('comment', 'None')),
                "Banned Users": "🚫 **{}**".format(', '.join(gameserver.get('general', {}).get('bans', '').splitlines() or ['None'])),
                "Game": "🎮 **{}**".format(gameserver.get('game_human', 'Unknown')),
                "Mission": "🏆 **{}**".format(gameserver.get('settings', {}).get('config', {}).get('mission', 'Unknown')),
                "3rd Person": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("disable3rdPerson", "1") == "0" else "❌ **Disabled**",
                "Crosshair": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("disableCrosshair", "1") == "0" else "❌ **Disabled**",
                "Shot Validation": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("shotValidation", "0") == "1" else "❌ **Disabled**",
                "Mouse and Keyboard": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("enableMouseAndKeyboard", "1") == "1" else "❌ **Disabled**",
                "Base Damage": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("disableBaseDamage", "1") == "0" else "❌ **Disabled**",
                "Container Damage": "✅ **Enabled**" if gameserver.get("settings", {}).get("config", {}).get("disableContainerDamage", "1") == "0" else "❌ **Disabled**",
                "Version": "📅 **{}**".format(gameserver.get('query', {}).get('version', 'Unknown')),
            }

            for key, value in properties.items():
                markdown_output += "| {} | {} |\n".format(key, value)

            markdown_output += "\n---\n\n"
        else:
            logging.warning("No gameserver details found for service ID {}.".format(service_id))

    return markdown_output

def get_services(api_key):
    """Fetch all services from the Nitrado API."""
    response = requests.get('https://api.nitrado.net/services', headers={'Authorization': 'Bearer {}'.format(api_key)})
    if response.ok:
        return response.json().get("data", {}).get("services", [])
    else:
        print("Error fetching services: {} - {}".format(response.status_code, response.text))
        return []

if __name__ == "__main__":
    # Fetch services and generate Markdown output
    services = get_services(API_KEY)
    markdown = generate_markdown(services, API_KEY)

    # Output the Markdown formatted result
    with open("README.md", "w") as file:
        file.write(markdown)

    print("Markdown output generated in README.md.")
