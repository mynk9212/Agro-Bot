import numpy as np
from sklearn.ensemble import RandomForestClassifier
import tensorflow as tf

def train_rf_model():
    X = np.random.rand(100, 4) * [40, 100, 1023, 100]
    y = np.random.randint(0, 2, 100)
    
    rf = RandomForestClassifier(n_estimators=100, random_state=42)
    rf.fit(X, y)
    return rf

def predict_environment(model, temp, humidity, moisture, distance):
    features = np.array([[temp, humidity, moisture, distance]])
    return model.predict(features)[0]

def load_mobilenet():
    model = tf.keras.applications.MobileNetV2(weights='imagenet')
    return model

def analyze_plant_image(model, image_path):
    img = tf.keras.preprocessing.image.load_img(image_path, target_size=(224, 224))
    x = tf.keras.preprocessing.image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = tf.keras.applications.mobilenet_v2.preprocess_input(x)
    preds = model.predict(x)
    decoded = tf.keras.applications.mobilenet_v2.decode_predictions(preds, top=3)[0]
    return [str(d[1]) for d in decoded]
