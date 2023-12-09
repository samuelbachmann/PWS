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

    for item in lines:
        key, value = item.split(':')
        if key == 'GPS':
            current_gps = value
        elif key == 'MQ135' and current_gps:
            result[current_gps] = value.split(',')

def createGPXTrack():
    gpx = gpxpy.gpx.GPX()

    track = gpxpy.gpx.GPXTrack()
    gpx.tracks.append(track)

    # Create a track segment
    segment = gpxpy.gpx.GPXTrackSegment()
    track.segments.append(segment)

    # Create track points with datetime objects
    time_format = "%Y-%m-%dT%H:%M:%SZ"
    time1 = datetime.strptime("2023-12-09T12:00:00Z", time_format)
    point1 = gpxpy.gpx.GPXTrackPoint(latitude=42.1234, longitude=-71.5678, time=time1)
    segment.points.append(point1)

    time2 = datetime.strptime("2023-12-09T12:05:00Z", time_format)
    point2 = gpxpy.gpx.GPXTrackPoint(latitude=42.2345, longitude=-71.6789, time=time2)
    segment.points.append(point2)

def saveGPXFile(gpx):
    with open("new_file.gpx", "w") as file:
        file.write(gpx.to_xml())   

main()