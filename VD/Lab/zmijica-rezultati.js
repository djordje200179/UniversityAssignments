$(document).ready(() => {
	const scores = JSON.parse(localStorage.getItem("scores")) ?? [];

	const topScores = scores.sort((a, b) => b.score - a.score).slice(0, 5);
	console.log(topScores);

	const table = $("#table_body");
	for (const index in topScores) {
		const row = $('<tr>');

		const indexCell = $('<td>').text(Number(index) + 1);
		const nameCell = $('<td>').text(topScores[index].name);
		const scoreCell = $('<td>').text(topScores[index].score);

		row.append(indexCell);
		row.append(nameCell);
		row.append(scoreCell);

		table.append(row);
	}
});