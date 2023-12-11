from flask import Flask, render_template, request, send_file
import gpxpy.gpx
from datetime import datetime

app = Flask(__name__)

@app.route('/')
def upload_file():
    return '''
    <html>
       <body>
          <h1>Upload a data file</h1>
          <form action="/convert" method="post" enctype="multipart/form-data">
             <input type="file" name="file" accept=".txt">
             <input type="submit" value="Convert to GPX">
          </form>
       </body>
    </html>
    '''

@app.route('/convert', methods=['POST'])
def convert_file():
    uploaded_file = request.files['file']
    if uploaded_file.filename != '':
        data = uploaded_file.read().decode("utf-8")
        result = sort_values(data)
        gpx = create_gpx_track(result)
        return send_file(gpx.to_xml(), as_attachment=True, mimetype='application/gpx+xml')

    return 'No file uploaded'
def sort_values(data):
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

    return result

def create_gpx_track(result):
    gpx = gpxpy.gpx.GPX()
    track = gpxpy.gpx.GPXTrack()
    gpx.tracks.append(track)
    segment = gpxpy.gpx.GPXTrackSegment()
    track.segments.append(segment)

    for key, value in result.items():
        try:
            segment.points.append(create_track_points(key, value['Date']))
        except ValueError as e:
            print(f"Error converting date '{value['Date']}': {str(e)}")
            # Handle the error here, e.g., skip this point or log it

    return gpx

def create_track_points(key, date):
    time_formats = [
        '%d-%m-%YT%H:%M:%SZ',
        # Add more date formats as needed, based on potential variations
        '%Y-%m-%dT%H:%M:%SZ'
    ]
    latitude, longitude = key.split(",")

    for time_format in time_formats:
        try:
            date_value = datetime.strptime(date, time_format)
            point = gpxpy.gpx.GPXTrackPoint(latitude=latitude, longitude=longitude, time=date_value)
            return point
        except ValueError:
            pass

    raise ValueError(f"Unrecognized date format for date '{date}'")

if __name__ == '__main__':
    app.run(debug=True)
