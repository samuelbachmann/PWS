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
    current_gps = None

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
    i = 0

    track = gpxpy.gpx.GPXTrack()
    gpx.tracks.append(track)
    segment = gpxpy.gpx.GPXTrackSegment()
    track.segments.append(segment)

    for key, value in result.items():
        segment.points.append(createTrackPoints(key, value['Date']))
        i += 1 

    return gpx

def createTrackPoints(key, date):
    timeFormat = '%m-%d-%YT%H:%M:%SZ'

    latitude, longitude = key.split(",")

    dateValue = datetime.strptime(date, timeFormat)
    point = gpxpy.gpx.GPXTrackPoint(latitude=latitude, longitude=longitude, time=dateValue)
    
    return point

def saveGPXFile(gpx, file_name='GPSData.gpx'):
    with open(file_name, "w") as file:  # Open the file in write text mode
        file.write(gpx.to_xml())  # Write the GPX data to the file

    return file_name
