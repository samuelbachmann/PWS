import gpxpy
import gpxpy.gpx
from datetime import datetime

# Create a GPX object
gpx = gpxpy.gpx.GPX()

# Create a track segment
track = gpxpy.gpx.GPXTrack()
gpx.tracks.append(track)
segment = gpxpy.gpx.GPXTrackSegment()
track.segments.append(segment)

# Define time objects
time1 = datetime.strptime("2023-12-08T08:00:00Z", "%Y-%m-%dT%H:%M:%SZ")
time2 = datetime.strptime("2023-12-08T08:10:00Z", "%Y-%m-%dT%H:%M:%SZ")
time3 = datetime.strptime("2023-12-08T08:20:00Z", "%Y-%m-%dT%H:%M:%SZ")

# Add points to the track segment with datetime objects
segment.points.append(gpxpy.gpx.GPXTrackPoint(37.1234, -122.4567, elevation=100, time=time1))
segment.points.append(gpxpy.gpx.GPXTrackPoint(37.2345, -122.5678, elevation=120, time=time2))
segment.points.append(gpxpy.gpx.GPXTrackPoint(37.3456, -122.6789, elevation=90, time=time3))

# Save the GPX data to a file
with open("example.gpx", "w") as f:
    f.write(gpx.to_xml())
