import streamlit as st
import pandas as pd
import folium
from streamlit_folium import st_folium
from ml_models import train_rf_model, predict_environment
import random

st.set_page_config(layout="wide", page_title="Agro Robo Dashboard")

@st.cache_resource
def get_model():
    return train_rf_model()

rf_model = get_model()

st.title("Agro Robo Dashboard")

temp = round(random.uniform(20.0, 35.0), 1)
humidity = round(random.uniform(40.0, 80.0), 1)
moisture = random.randint(300, 800)
distance = random.randint(10, 100)
lat = 12.9716 + random.uniform(-0.01, 0.01)
lng = 77.5946 + random.uniform(-0.01, 0.01)

col1, col2, col3, col4 = st.columns(4)
col1.metric("Temperature (°C)", temp)
col2.metric("Humidity (%)", humidity)
col3.metric("Soil Moisture", moisture)
col4.metric("Obstacle Distance (cm)", distance)

prediction = predict_environment(rf_model, temp, humidity, moisture, distance)
status = "Watering Needed" if prediction == 1 else "Optimal Conditions"
st.subheader(f"Environment Status: {status}")

st.markdown("### Live Robot Location")
m = folium.Map(location=[lat, lng], zoom_start=15)
folium.Marker([lat, lng], popup="Agro Robo").add_to(m)
st_folium(m, width=800, height=400)

if st.button("Refresh Data"):
    st.rerun()
