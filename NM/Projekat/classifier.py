from keras import Sequential
import keras.callbacks as callbacks
import keras.layers as layers
from keras.optimizers import Adam
from keras.utils import image_dataset_from_directory


class DogBreedsClassifier:
	__image_size = (128, 128)
	__batch_size = 64
	__learning_rate = 0.001
	__early_stopping_patience = 10
	__max_epochs = 1000
	__validation_split = 0.2
	__rng_seed = 20

	__model: Sequential
	__image_colors: int
	__num_of_classes: int

	def __init__(self, image_colors: int, num_of_classes: int):
		self.__image_colors = image_colors
		self.__num_of_classes = num_of_classes

		preprocessing_layers = Sequential([
			# layers.Rescaling(1. / 255),
			layers.RandomFlip("horizontal"),
			layers.RandomRotation(0.25),
			layers.RandomZoom(0.1),
			layers.Rescaling(1. / 255)
		])

		convolutional_layers = Sequential([
			layers.Conv2D(16, 3, padding='same', activation='relu'),
			layers.MaxPooling2D(),

			layers.Conv2D(32, 3, padding='same', activation='relu'),
			layers.MaxPooling2D(),

			layers.Conv2D(64, 3, padding='same', activation='relu'),
			layers.MaxPooling2D(),
		])

		fully_connected_layers = Sequential([
			layers.Dense(128, activation='relu'),
			layers.Dense(num_of_classes, activation="softmax")
		])

		self.__model = Sequential([
			layers.InputLayer(input_shape=(*DogBreedsClassifier.__image_size, image_colors)),
			preprocessing_layers,
			convolutional_layers,
			layers.Flatten(),
			fully_connected_layers
		])

		optimizer = Adam(learning_rate=DogBreedsClassifier.__learning_rate)
		self.__model.compile(
			optimizer=optimizer,
			loss="sparse_categorical_crossentropy",
			metrics="accuracy"
		)

	@staticmethod
	def __load_training_data(train_data_path: str, validation_data_path: str):
		if validation_data_path is None:
			train_data, validation_data = image_dataset_from_directory(
				train_data_path,
				image_size=DogBreedsClassifier.__image_size,
				batch_size=DogBreedsClassifier.__batch_size,
				validation_split=DogBreedsClassifier.__validation_split, seed=DogBreedsClassifier.__rng_seed,
				subset="both"
			)
		else:
			train_data = image_dataset_from_directory(
				train_data_path,
				image_size=DogBreedsClassifier.__image_size,
				batch_size=DogBreedsClassifier.__batch_size
			)

			validation_data = image_dataset_from_directory(
				validation_data_path,
				image_size=DogBreedsClassifier.__image_size,
				batch_size=DogBreedsClassifier.__batch_size
			)

		return train_data, validation_data

	def train(self, train_data_path: str, validation_data_path: str = None, verbose=True):
		train_data, validation_data = self.__load_training_data(train_data_path, validation_data_path)

		training_callbacks = [
			callbacks.TensorBoard(log_dir="logs"),
			callbacks.EarlyStopping(
				monitor="val_accuracy", patience=DogBreedsClassifier.__early_stopping_patience,
				restore_best_weights=True,
				verbose=1 if verbose else 0
			),
			callbacks.ModelCheckpoint(
				filepath="models/model",
				monitor="val_accuracy",
				save_best_only=True,
				verbose=1 if verbose else 0
			)
		]

		self.__model.fit(
			train_data, validation_data=validation_data,
			epochs=DogBreedsClassifier.__max_epochs,
			callbacks=training_callbacks,
			verbose=2 if verbose else 0
		)

		self.__model.save('models/model')
