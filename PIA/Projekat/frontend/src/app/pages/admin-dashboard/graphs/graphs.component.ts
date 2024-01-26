import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {CanvasJSAngularChartsModule} from "@canvasjs/angular-charts";
import {StatsService} from "../../../services/stats.service";


interface PieChartRecord {
	y: number;
	name: string;
}

interface BarChartRecord {
	label: string;
	y: number;
}

interface LineChartRecord {
	x: number;
	y: number;
}

@Component({
	selector: "app-graphs",
	standalone: true,
	imports: [CommonModule, CanvasJSAngularChartsModule],
	templateUrl: "./graphs.component.html",
	styleUrls: ["./graphs.component.scss"]
})
export class GraphsComponent implements OnInit {
	public studentGenderChartOptions = {};
	public teacherGenderChartOptions = {};
	public teacherAgeGenderChartOptions = {};
	public classesPerDaysChartOptions = {};
	public topTeachersChartOptions = {};
	public classStatusesChartOptions = {};
	public teacherStatusesChartOptions = {};

	private static readonly genderTranslations : {[key: string]: string} = {
		male: "Мушкарци",
		female: "Жене",
	};

	private static readonly dayNames = [
		"Понедељак", "Уторак", "Сриједа", "Четвртак",
		"Петак", "Субота", "Недеља",
	];

	private static readonly defaultChartOptions = {
		animationEnabled: true,
		theme: "dark1",
		backgroundColor: "transparent",
	}

	public constructor(private readonly statsService: StatsService) {

	}

	public ngOnInit(): void {
		this.statsService.getSubjectAgeCounters().subscribe(
			subjectAgeCounters => {
				const chartData = [];

				for (const ageGroup in subjectAgeCounters) {
					const dataPoints = [] as BarChartRecord[];

					for (const subject in subjectAgeCounters[ageGroup]) {
						const count = subjectAgeCounters[ageGroup][subject];
						dataPoints.push({
							label: subject,
							y: count,
						});
					}

					chartData.push({
						type: "column",
						name: ageGroup,
						legendText: ageGroup,
						showInLegend: true,
						dataPoints: dataPoints,
					});
				}

				this.teacherAgeGenderChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Наставници према предмету и узрасту",
					},
					data: chartData,
				};
			},
			console.error
		);

		this.statsService.getStudentCountByGender().subscribe(
			genderCounters => {
				const chartData = [] as PieChartRecord[];

				for (const gender in genderCounters) {
					const count = genderCounters[gender];
					chartData.push({
						name: GraphsComponent.genderTranslations[gender],
						y: count,
					})
				}

				this.studentGenderChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Студенти према полу"
					},
					data: [{
						type: "pie",
						startAngle: 45,
						indexLabel: "{name}: {y}",
						indexLabelPlacement: "inside",
						yValueFormatString: "#,###",
						dataPoints: chartData
					}]
				};
			},
			console.error
		);

		this.statsService.getTeacherCountByGender().subscribe(
			genderCounters => {
				const chartData = [] as PieChartRecord[];

				for (const gender in genderCounters) {
					const count = genderCounters[gender];
					chartData.push({
						name: GraphsComponent.genderTranslations[gender],
						y: count,
					})
				}

				this.teacherGenderChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Наставници према полу"
					},
					data: [{
						type: "pie",
						startAngle: 45,
						indexLabel: "{name}: {y}",
						indexLabelPlacement: "inside",
						yValueFormatString: "#,###",
						dataPoints: chartData
					}]
				};
			},
			console.error
		);

		this.statsService.getClassesPerDays().subscribe(
			counters => {
				const graphData = [] as BarChartRecord[];
				for (let i = 0; i < 7; i++) {
					graphData.push({
						label: GraphsComponent.dayNames[i],
						y: counters[i]
					});
				}


				this.classesPerDaysChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Часови по дану"
					},
					data: [{
						type: "column",
						dataPoints: graphData
					}]
				};
			},
			console.error
		);

		this.statsService.getTopTeachers().subscribe(
			topTeachers => {
				const chartData = [];

				for (const teacherInfo of topTeachers) {
					const dataPoints = [] as LineChartRecord[];

					for (let i = 0; i < 12; i++) {
						const count = teacherInfo.classesPerMonth[i];

						dataPoints.push({ x: i + 1, y: count});
					}

					const teacherName = `${teacherInfo.teacher.info.firstName} ${teacherInfo.teacher.info.lastName}`;

					chartData.push({
							type: "line",
							name: teacherName,
							showInLegend: true,
							dataPoints: dataPoints,
						}
					);
				}

				this.topTeachersChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Најактивнији наставници"
					},
					axisY: {
						title: "Број часова"
					},
					axisX: {
						title: "Мјесец"
					},
					data: chartData,
				};
			},
			console.error
		);

		this.statsService.getClassStatuses().subscribe(
			statuses => {
				const chartData = [] as PieChartRecord[];

				for (const status in statuses) {
					const count = statuses[status];
					chartData.push({
						name: status,
						y: count,
					})
				}

				this.classStatusesChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Часови према статусу"
					},
					data: [{
						type: "doughnut",
						startAngle: 45,
						indexLabel: "{name}: {y}",
						indexLabelPlacement: "inside",
						yValueFormatString: "#,###",
						dataPoints: chartData
					}]
				};
			},
			console.error
		);

		this.statsService.getTeacherStatuses().subscribe(
			statuses => {
				const chartData = [] as PieChartRecord[];

				for (const status in statuses) {
					const count = statuses[status];
					chartData.push({
						name: status,
						y: count,
					})
				}

				this.teacherStatusesChartOptions = {
					...GraphsComponent.defaultChartOptions,
					title:{
						text: "Наставници према статусу"
					},
					data: [{
						type: "doughnut",
						startAngle: 45,
						indexLabel: "{name}: {y}",
						indexLabelPlacement: "inside",
						yValueFormatString: "#,###",
						dataPoints: chartData
					}]
				};
			},
			console.error
		);
	}
}
