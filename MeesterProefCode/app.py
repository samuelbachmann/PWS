from flask import Flask, render_template, request, Response
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
        gpx_xml = gpx.to_xml()  # Convert GPX object to XML

        return Response(
            gpx_xml,
            mimetype="application/gpx+xml",
            headers={"Content-disposition": "attachment; filename=converted.gpx"}
        )
    else:
        return "No file uploaded."


if __name__ == '__main__':
    app.run(debug=True)
