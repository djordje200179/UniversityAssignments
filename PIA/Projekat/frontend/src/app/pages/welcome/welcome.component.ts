import {Component} from "@angular/core";
import {StatsComponent} from "./stats/stats.component";
import {EnrollmentsComponent} from "./enrollments/enrollments.component";

@Component({
	selector: "app-welcome",
	templateUrl: "./welcome.component.html",
	styleUrls: ["./welcome.component.scss"],
	standalone: true,
	imports: [
		StatsComponent,
		EnrollmentsComponent
	]
})
export class WelcomeComponent {

}
