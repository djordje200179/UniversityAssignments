let canvas;
let canvasContext;

let WIDTH, HEIGHT;
let PIXEL_SIZE = 20;
let UPDATE_INTERVAL = 400;

const BACKGROUND_COLOR = "#000000";
const HEAD_COLOR = "#00FF00";
const BODY_COLOR = "#008000";
const FOOD_COLOR = "#FF5555";
const SUPERFOOD_COLOR = "#FF0000";

function setupCanvas() {
	canvas = $("canvas")[0];
	canvas.focus();

	canvasContext = canvas.getContext("2d");
	canvasContext.translate(canvas.width / 2, canvas.height / 2);

	PIXEL_SIZE = canvas.width / WIDTH;
}

const KEY_UP = 38;
const KEY_DOWN = 40;
const KEY_LEFT = 37;
const KEY_RIGHT = 39;
const KEY_SPACE = 32;

let running = false;
let updateIntervalID, superFoodGenerationIntervalID, superFoodRemovalTimeoutID

$(document).ready(() => {
	const urlParams = new URLSearchParams(window.location.search);
	const mapSize = urlParams.get("map_size");
	const difficulty = Number(urlParams.get("difficulty"));

	WIDTH = HEIGHT = mapSize;
	UPDATE_INTERVAL = 800 / 2 ** difficulty;

	setupCanvas();

	$(document).keydown(event => {
		switch (event.keyCode) {
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			changeDirection(event.keyCode);
			break;
		case KEY_SPACE:
			if (!running)
				resume();
			else
				pause();

			break;
		}
	});

	bestScore = Number(localStorage.getItem("best_score"));
	$("#best_score_text").text(`Najbolji rezultat: ${bestScore}`);

	resume();

	food = generateFood();
	superFood = null;
});

function resume() {
	updateIntervalID = setInterval(update, UPDATE_INTERVAL);
	superFoodGenerationIntervalID = setInterval(() => {
		if (superFood === null)
			superFood = generateFood();

		superFoodRemovalTimeoutID = setTimeout(() => superFood = null, 4000);
	}, 10000);
	running = true;
}

function pause() {
	clearInterval(updateIntervalID);
	clearInterval(superFoodGenerationIntervalID);
	clearTimeout(superFoodRemovalTimeoutID);

	running = false;
}

function end() {
	pause();

	const choice = confirm("Izgubili ste! Želite li da sačuvate rezultat?");
	if(choice) {
		const name = prompt("Unesite vaše ime:");

		const results = JSON.parse(localStorage.getItem("scores")) ?? [];
		results.push({
			name,
			score: currentScore
		});
		localStorage.setItem("scores", JSON.stringify(results));

		if(currentScore > bestScore)
			localStorage.setItem("best_score", currentScore);
	}

	window.location.href = "zmijica-rezultati.html";
}

let snakeParts = [[0, 0]];
let currentDirection = [0, 1];
let plannedDirection = currentDirection;
let food = [0, 0];
let superFood = [0, 0];
let currentScore = 0;
let bestScore;

function changeDirection(directionKey) {
	let newDirection;
	switch (directionKey) {
	case KEY_UP:
		newDirection = [0, -1];
		break;
	case KEY_DOWN:
		newDirection = [0, 1];
		break;
	case KEY_LEFT:
		newDirection = [-1, 0];
		break;
	case KEY_RIGHT:
		newDirection = [1, 0];
		break;
	}

	if (((currentDirection[0] === newDirection[0] && currentDirection[1] === newDirection[1]) ||
		(currentDirection[0] === -newDirection[0] && currentDirection[1] === -newDirection[1])))
		return;

	plannedDirection = newDirection;
}

function update() {
	updatePosition();
	checkCollisions();

	drawSnake();

	checkEat();
	drawFood();

	$("#current_score_text").text(`Trenutni rezultat: ${currentScore}`);

	const percentage = currentScore > bestScore ? 100.0 : currentScore / bestScore * 100.0;
	$("#progress_bar").width(`${percentage}%`).text(`${currentScore} / ${bestScore}`);
}

function updatePosition() {
	const newHead = [snakeParts[0][0] + currentDirection[0], snakeParts[0][1] + currentDirection[1]];
	snakeParts.unshift(newHead);
	snakeParts.pop();

	currentDirection = plannedDirection;
}

function drawSnake() {
	canvasContext.save();
	canvasContext.setTransform(1, 0, 0, 1, 0, 0);
	canvasContext.fillStyle = BACKGROUND_COLOR;
	canvasContext.fillRect(0, 0, canvas.width, canvas.height);
	canvasContext.restore();

	const head = snakeParts[0];

	canvasContext.fillStyle = HEAD_COLOR;
	canvasContext.fillRect(
		head[0] * PIXEL_SIZE, head[1] * PIXEL_SIZE,
		PIXEL_SIZE, PIXEL_SIZE
	)

	canvasContext.fillStyle = BODY_COLOR;

	if (snakeParts.length > 2) {
		const body = snakeParts.slice(1, -1);

		body.forEach(part => canvasContext.fillRect(
			part[0] * PIXEL_SIZE, part[1] * PIXEL_SIZE,
			PIXEL_SIZE, PIXEL_SIZE
		));
	}

	if (snakeParts.length > 1) {
		const tail = snakeParts[snakeParts.length - 1];

		const previous = snakeParts[snakeParts.length - 2];
		const direction = [previous[0] - tail[0], previous[1] - tail[1]];

		canvasContext.beginPath();
		if(direction[0] === 1) {
			canvasContext.moveTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 0.5) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 0.5) * PIXEL_SIZE);
		} else if(direction[0] === -1) {
			canvasContext.moveTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 0.5) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 0.5) * PIXEL_SIZE);
		} else if(direction[1] === 1) {
			canvasContext.moveTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0.5) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
		} else if(direction[1] === -1) {
			canvasContext.moveTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0.5) * PIXEL_SIZE, (tail[1] + 1) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 1) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
			canvasContext.lineTo((tail[0] + 0) * PIXEL_SIZE, (tail[1] + 0) * PIXEL_SIZE);
		}

		canvasContext.fill();
	}
}

function drawFood() {
	canvasContext.fillStyle = FOOD_COLOR;
	canvasContext.beginPath();
	canvasContext.arc(
		(food[0] + 0.5) * PIXEL_SIZE, (food[1] + 0.5) * PIXEL_SIZE,
		PIXEL_SIZE / 2, 0, 2 * Math.PI
	);
	canvasContext.fill();

	if (superFood) {
		canvasContext.fillStyle = SUPERFOOD_COLOR;
		canvasContext.beginPath();
		canvasContext.arc(
			(superFood[0] + 0.5) * PIXEL_SIZE, (superFood[1] + 0.5) * PIXEL_SIZE,
			PIXEL_SIZE / 4 * 3, 0, 2 * Math.PI
		);
		canvasContext.fill();
	}
}

function checkCollisions() {
	const head = snakeParts[0];
	const body = snakeParts.slice(1, -1);

	if ((body.some(part => part[0] === head[0] && part[1] === head[1]) ||
		(head[0] < -WIDTH / 2 || head[0] >= WIDTH / 2 || head[1] < -HEIGHT / 2 || head[1] >= HEIGHT / 2)))
		end();
}

function checkEat() {
	const head = snakeParts[0];

	if (head[0] === food[0] && head[1] === food[1]) {
		snakeParts.push(snakeParts[snakeParts.length - 1]);
		food = generateFood();
		currentScore++;
	}

	if(superFood && head[0] === superFood[0] && head[1] === superFood[1]) {
		snakeParts.push(snakeParts[snakeParts.length - 1]);
		superFood = null;

		currentScore += 10;
	}
}

function generateFood() {
	let xCord, yCord;

	while (true) {
		xCord = Math.floor(Math.random() * WIDTH) - WIDTH >> 1;
		yCord = Math.floor(Math.random() * HEIGHT) - HEIGHT >> 1;

		if (!snakeParts.some(part => part[0] === xCord && part[1] === yCord))
			break;
	}

	return [xCord, yCord];
}