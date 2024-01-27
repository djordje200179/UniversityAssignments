import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {UserInfo, UsersService} from "../../services/users.service";
import {StudentArchiveComponent} from "./student-archive/student-archive.component";
import {StudentUpcomingComponent} from "./student-upcoming/student-upcoming.component";
import {TeacherUpcomingComponent} from "./teacher-upcoming/teacher-upcoming.component";
import {TeacherRequestedComponent} from "./teacher-requested/teacher-requested.component";
import {TeacherCalendarComponent} from "./teacher-calendar/teacher-calendar.component";

@Component({
	selector: "app-classes",
	standalone: true,
	imports: [
		CommonModule,
		StudentArchiveComponent,
		StudentUpcomingComponent,
		TeacherUpcomingComponent,
		TeacherRequestedComponent,
		TeacherCalendarComponent
	],
	templateUrl: "./classes.component.html",
	styleUrls: ["./classes.component.scss"]
})
export class ClassesComponent {
	public readonly currentUser: UserInfo;

	public constructor(private readonly usersService: UsersService) {
		this.currentUser = this.usersService.getCurrentUser()!;
	}
}
