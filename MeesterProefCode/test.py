import gpxpy
import gpxpy.gpx
from datetime import datetime

# Create a new GPX object
gpx = gpxpy.gpx.GPX()

# Create a waypoint
wpt = gpxpy.gpx.GPXWaypoint(latitude=42.1234, longitude=-71.5678)
wpt.name = "MyWaypoint"
wpt.description = "This is a custom waypoint"
gpx.waypoints.append(wpt)

# Create a track
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

# Save the GPX data to a file
with open("new_file.gpx", "w") as file:
    file.write(gpx.to_xml())
