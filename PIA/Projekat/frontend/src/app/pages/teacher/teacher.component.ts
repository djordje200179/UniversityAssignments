import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {ActivatedRoute} from "@angular/router";
import {TeachersService} from "../../services/teachers.service";
import {TeacherInfo, UsersService} from "../../services/users.service";
import {InfoComponent} from "./info/info.component";
import {RatingsComponent} from "./ratings/ratings.component";
import {SubjectsComponent} from "./subjects/subjects.component";
import {ScheduleFormComponent} from "./schedule-form/schedule-form.component";

@Component({
	selector: "app-teacher",
	standalone: true,
	imports: [CommonModule, InfoComponent, RatingsComponent, SubjectsComponent, ScheduleFormComponent],
	templateUrl: "./teacher.component.html",
	styleUrls: ["./teacher.component.scss"]
})
export class TeacherComponent implements OnInit {
	public readonly teacherUsername: string;

	public teacherInfo?: TeacherInfo;
	public teacherSubjects?: string[];

	public constructor(
		private readonly activatedRoute: ActivatedRoute,
		private readonly usersService: UsersService,
		private readonly teachersService: TeachersService
	) {
		this.teacherUsername = this.activatedRoute.snapshot.params["username"];
	}

	public ngOnInit(): void {
		this.usersService.getTeacherInfo(this.teacherUsername).subscribe(
			teacherInfo => this.teacherInfo = teacherInfo,
			console.error
		);

		this.teachersService.getTeacherEnrollments(this.teacherUsername).subscribe(
			subjects => this.teacherSubjects = subjects.map(enrollment => enrollment.subject),
			console.error
		);
	}
}
