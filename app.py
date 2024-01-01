from flask import Flask, render_template, request
import xgboost as xgb
import pandas as pd
import numpy as np

app = Flask(__name__)

# Load the XGBoost model
model = xgb.Booster()
model.load_model('fish_species_model.model')

# Define the mapping of fish levels
fish_levels = {
    0: "tilapia",
    1: "rui",
    2: "pangas",
    3: "katla",
    4: "silverCup",
    5: "shrimp",
    6: "sing",
    7: "karpio",
    8: "koi",
    9: "prawn",
    10: "magur"
}

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/', methods=['POST'])
def predict():
    # Get user input
    ph = float(request.form['ph'])
    temperature = float(request.form['temperature'])
    turbidity = float(request.form['turbidity'])

    # Create a DataFrame from user input
    input_data = pd.DataFrame({'ph': [ph], 'temperature': [temperature], 'turbidity': [turbidity]})

    # Predict the fish species
    prediction = model.predict(xgb.DMatrix(input_data))
    fish_species = np.argmax(prediction)

    # Get the name of the predicted fish species
    predicted_species = fish_levels.get(fish_species, "Unknown")

    return render_template('index.html', prediction=predicted_species)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
