import re
import folium

# Read data from the file
with open('data.txt', 'r') as file:
    data = file.readlines()

# Extract GPS values from the data
gps_pattern = r'GPS: (\d+\.\d+), (\d+\.\d+)'
gps_data = []
for line in data:
    match = re.search(gps_pattern, line)
    if match:
        latitude = float(match.group(1))
        longitude = float(match.group(2))
        gps_data.append((latitude, longitude))

# Create a map using Folium
mymap = folium.Map(location=gps_data[0], zoom_start=12)

# Plot GPS points on the map
for point in gps_data:
    folium.Marker(point).add_to(mymap)

# Save the map to an HTML file
mymap.save('gps_map.html')

# pip install matplotlib folium
