from flask import Flask, render_template, request, send_file
from convertTXTtoGPX import processUploadedData, saveGPXFile
from io import BytesIO

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('upload_form.html')


@app.route('/convert', methods=['POST'])
def convert():
    uploaded_file = request.files['file']
    if uploaded_file.filename != '':
        file_data = uploaded_file.read().decode('utf-8')  # Extract text data
        gpx = processUploadedData(file_data)
        saveGPXFile(gpx, 'GPSData.gpx') 
        gpx_io = BytesIO()  # Create a BytesIO object to store GPX data
        gpx_io.seek(0)  # Reset the BytesIO object's pointer to the beginning
        return send_file(gpx_io, as_attachment=True, mimetype='application/gpx+xml', download_name='converted.gpx')
    else:
        return "No file uploaded."

if __name__ == '__main__':
    app.run(debug=True)
