
from flask import Flask, request, jsonify
import speech_recognition as sr
from datetime import datetime

app = Flask(__name__)
health_data = {}

@app.route('/healthdata', methods=['POST'])
def receive_data():
    content = request.json
    temp = content.get("temperature")
    humidity = content.get("humidity")
    systolic = content.get("systolic", 120)
    diastolic = content.get("diastolic", 80)
    heart_rate = content.get("heart_rate", 75)

    health_data.update({
        'timestamp': datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        'temperature': temp,
        'humidity': humidity,
        'systolic': systolic,
        'diastolic': diastolic,
        'heart_rate': heart_rate
    })
    return jsonify({"status": "Data received"}), 200

@app.route('/symptom', methods=['GET'])
def get_symptom():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("Speak your symptom:")
        audio = recognizer.listen(source)
        try:
            text = recognizer.recognize_google(audio)
            return jsonify({"symptom": text})
        except sr.UnknownValueError:
            return jsonify({"error": "Could not understand audio"})
        except sr.RequestError:
            return jsonify({"error": "NLP service error"})

@app.route('/report', methods=['GET'])
def report():
    return jsonify(health_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
