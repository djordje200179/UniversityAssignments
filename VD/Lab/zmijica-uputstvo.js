function startGame() {
	const configFormValues = $("#game_config_form")[0].elements;

	const mapSize = configFormValues["map_size"];
	const difficulty = configFormValues["difficulty"];

	if (mapSize === 0 || difficulty === 0)
		return false;
}