import os
import logging
import numpy as np
import pandas as pd
import tensorflow as tf
import matplotlib.pyplot as plt
from keras import Sequential
from keras import layers, regularizers, callbacks
from keras.losses import SparseCategoricalCrossentropy
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.utils import image_dataset_from_directory
from sklearn.utils import class_weight
from sklearn.metrics import accuracy_score, confusion_matrix, ConfusionMatrixDisplay

tf.get_logger().setLevel(logging.ERROR)

image_shape = (128, 128)
batch_size = 128

def get_num_of_images(img_name_dir: str, dir_path: str):
	whole_path = f"{dir_path}/{img_name_dir}"
	files = os.listdir(whole_path)
	return len(files)

def draw_hist(classes:list, train_path, test_path, valid_path):
	data_dict = {name: 0 for name in classes}
	for name in classes:
		data_dict[name] += get_num_of_images(name, train_path)
		data_dict[name] += get_num_of_images(name, test_path)
		data_dict[name] += get_num_of_images(name, valid_path)

	plt.bar(data_dict.keys(), data_dict.values(), edgecolor='black', width=0.1, color='black')
	plt.tick_params(axis='x', labelrotation=90, labelsize=6)
	fig,ax = plt.subplots(figsize=(35,35,1))
	ax.title("dog classification into breeds")
	ax.xlabel("breeds")
	ax.ylabel("number of data per breed")
	plt.show()

def make_model():
	augmentation_model = Sequential([
		layers.RandomFlip("horizontal"),
		layers.RandomRotation(0.20),
		layers.RandomZoom(0.4),
		layers.RandomContrast(0.4)
	])

	conv_model = Sequential([
		layers.Conv2D(32, 7, padding='same', activation='relu'),
		layers.MaxPooling2D(),

		layers.Conv2D(64, 5, padding='same', activation='relu'),
		layers.MaxPooling2D(),

		layers.Conv2D(128, 3, padding='same', activation='relu'),
		layers.MaxPooling2D(),

		layers.Conv2D(256, 3, padding='same', activation='relu'),
		layers.MaxPooling2D(),
	])

	fc_model = Sequential([
		layers.Dense(2048, activation='relu', kernel_regularizer=regularizers.L2(1e-4)),
		layers.Dropout(0.3),

		layers.Dense(512, activation='relu'),

		layers.Dense(70, activation='softmax')
	])

	model = Sequential([
		layers.InputLayer(input_shape=(*image_shape, 3)),

		augmentation_model,

		layers.Rescaling(1. / 255),
		conv_model,

		layers.Flatten(),
		fc_model
	])

	model.summary()

	optimizer = Adam(learning_rate=0.001)
	model.compile(
		optimizer,
		loss="sparse_categorical_crossentropy",
		metrics='accuracy'
	)

	return model


train_path = "/kaggle/input/70-dog-breedsimage-data-set/train"
test_path = "/kaggle/input/70-dog-breedsimage-data-set/test"
valid_path = "/kaggle/input/70-dog-breedsimage-data-set/valid"

input_train = image_dataset_from_directory(train_path, image_size=image_shape, batch_size=batch_size)
input_test = image_dataset_from_directory(test_path, image_size=image_shape, batch_size=batch_size)
input_valid = image_dataset_from_directory(valid_path, image_size=image_shape, batch_size=batch_size)

all_classes = input_train.class_names
#calculate_hist_data(all_classes, train_path, test_path, valid_path)
# print(classes)

num_images_per_class = [
	len(os.listdir(f"/kaggle/input/70-dog-breedsimage-data-set/train/{label}"))
	for label in all_classes
]

y_train = [
	all_classes[i]
	for i, num_images in enumerate(num_images_per_class)
	for _ in range(num_images)
]

weights = class_weight.compute_class_weight(class_weight='balanced', classes=all_classes, y=np.asarray(y_train))
weights = {
	i: weight
	for i, weight in enumerate(weights)
}

cb = [
	callbacks.EarlyStopping(monitor='val_accuracy', patience=15, verbose=1, restore_best_weights=True),
	callbacks.TensorBoard(log_dir="logs")
]

model = make_model()
history = model.fit(input_train, validation_data=input_valid,
                    verbose=2, callbacks=cb,
                    epochs=200, shuffle=True, class_weight=weights)

model.save("/kaggle/working/model.h5")

acc = history.history['accuracy']
val_acc = history.history['val_accuracy']
loss = history.history['loss']
val_loss = history.history['val_loss']

plt.figure()
plt.subplot(121)
plt.plot(acc)
plt.plot(val_acc)
plt.title('Accuracy')
plt.subplot(122)
plt.plot(loss)
plt.plot(val_loss)
plt.title('Loss')
plt.show()


labels = np.array([])
pred = np.array([])
for img, lab in input_train:
	labels = np.append(labels, lab)
	pred = np.append(pred, np.argmax(model.predict(img, verbose=0), axis=1))
print('Tačnost modela je: ' + str(100*accuracy_score(labels, pred)) + '%')

cm = confusion_matrix(labels, pred, normalize='true')
cmDisplay = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=all_classes)
fig, ax = plt.subplots(figsize=(35,35))
cmDisplay.plot(ax=ax, xticks_rotation='vertical')
plt.show()