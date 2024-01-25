import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {CanvasJSAngularChartsModule} from "@canvasjs/angular-charts";
import {Role, UsersService} from "../../../services/users.service";
import {Router} from "@angular/router";
import {StatsService} from "../../../services/stats.service";


interface PieChartRecord {
	y: number;
	name: string;
}

interface BarChartRecord {
	label: string;
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

	private static readonly genderTranslations : {[key: string]: string} = {
		male: "Мушкарци",
		female: "Жене",
	};

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
	}
}
