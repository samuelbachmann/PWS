import gpxpy
import gpxpy.gpx
from datetime import datetime

def processUploadedData(data):
    result = sortValues(data)
    gpx = createGPXTrack(result)
    return gpx

def sortValues(data):
    lines = data.strip().replace('\r', '').split('\n')

    result = {}

    for line in lines:
        key, value = line.split(':', 1)
        if key == 'GPS':
            currentKey = value
            result[currentKey] = {}
        else:
            result[currentKey][key] = value

    return result

def createGPXTrack(result):
    gpx = gpxpy.gpx.GPX()

    track = gpxpy.gpx.GPXTrack()
    gpx.tracks.append(track)
    segment = gpxpy.gpx.GPXTrackSegment()
    track.segments.append(segment)

    for key, value in result.items():
        segment.points.append(createTrackPoints(key, value['Date']))

    return gpx

def createTrackPoints(key, date):
    timeFormat = '%m-%d-%YT%H:%M:%SZ'

    latitude, longitude = key.split(",")

    dateValue = datetime.strptime(date, timeFormat)
    point = gpxpy.gpx.GPXTrackPoint(latitude=latitude, longitude=longitude, time=dateValue)
    
    return point

def saveGPXFile(gpx, fileName='GPSData.gpx'):
    with open(fileName, "w") as file:
        file.write(gpx.to_xml())

    return fileName