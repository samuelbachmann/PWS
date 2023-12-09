import gpxpy
import gpxpy.gpx
from datetime import datetime

filePath = 'C:/Users/samue/Documents/GitHub/PWS/MeesterProefCode/data.txt'

def main():
    with open(filePath) as file:
        data = file.read()

    sortValues(data)

def sortValues(data):
    lines = data.strip().split('\n')

    result = {}
    current_gps = None

    for line in lines:
        key, value = line.split(':', 1)
        if key == 'GPS':
            current_key = value
            result[current_key] = {}
        else:
            result[current_key][key] = value

    createGPXTrack(result)

def createGPXTrack(result):
    gpx = gpxpy.gpx.GPX()

    track = gpxpy.gpx.GPXTrack()
    gpx.tracks.append(track)

    # Create a track segment
    segment = gpxpy.gpx.GPXTrackSegment()
    track.segments.append(segment)

    for key, value in result.items():
        segment.points.append(createTrackPoints(key, value['Date']))

    saveGPXFile(gpx)

def createTrackPoints(key, date):
    time_format = '%d-%m-%YT%H:%M:%SZ'

    latitude, longitude = key.split(",")

    dateValue = datetime.strptime(date, time_format)
    point = gpxpy.gpx.GPXTrackPoint(latitude=latitude, longitude=longitude, time=dateValue)
    
    return point

def saveGPXFile(gpx):
    with open("new_file.gpx", "w") as file:
        file.write(gpx.to_xml())   

main()