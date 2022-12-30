from classifier import DogBreedsClassifier


def main():
    classifier = DogBreedsClassifier(
        image_colors=3,
        num_of_classes=70
    )

    classifier.train(
        "data/train", "data/validation",
        verbose=True
    )


if __name__ == '__main__':
    main()
