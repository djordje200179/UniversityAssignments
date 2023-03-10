import os
import keras
import numpy as np
from keras import Sequential
from keras.utils import load_img, img_to_array, image_dataset_from_directory

model: Sequential = keras.models.load_model("model.h5")

img_path = "data/bzvz_test"

images = []
for img in os.listdir(img_path):
	img = os.path.join(img_path, img)
	img = load_img(img, target_size=(128, 128))
	img = img_to_array(img)
	img = np.expand_dims(img, axis=0)
	images.append(img)

classes = image_dataset_from_directory("data/test", image_size=(128, 128), batch_size=64).class_names
# stack up images list to pass for prediction

def print_top_5(prediction):
	indices = prediction.argsort()[-5:][::-1]
	for i in indices:
		print(f"{classes[i]}: {prediction[i] * 100:.2f}%", end='\t')
	print()


images = np.vstack(images)
predictions = model.predict(images, batch_size=10)
for prediction in predictions:
	print_top_5(prediction)