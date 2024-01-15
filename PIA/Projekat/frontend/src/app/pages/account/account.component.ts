import {Component, OnInit} from "@angular/core";
import {SignInFormComponent} from "./login/sign-in-form/sign-in-form.component";
import {SignUpFormComponent} from "./login/sign-up-form/sign-up-form.component";
import {Role, StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../services/users.service";
import {LoginComponent} from "./login/login.component";
import {NgIf} from "@angular/common";
import {EditAccountComponent} from "./edit-account/edit-account.component";

@Component({
	selector: "app-account",
	templateUrl: "./account.component.html",
	styleUrls: ["./account.component.scss"],
	imports: [
		SignInFormComponent,
		SignUpFormComponent,
		LoginComponent,
		NgIf,
		EditAccountComponent
	],
	standalone: true
})
export class AccountComponent implements OnInit {
	loggedUser: UserInfo | null;
	studentInfo?: StudentInfo;
	teacherInfo?: TeacherInfo;

	public constructor(private readonly usersService: UsersService) {
		this.loggedUser = usersService.getCurrentUser();
	}

	public ngOnInit(): void {
		switch (this.loggedUser?.role) {
		case undefined:
			break;
		case Role.Student:
			this.usersService.getStudentInfo(this.loggedUser.username!).subscribe(
				studentInfo => {
					this.studentInfo = studentInfo;
					localStorage.setItem("student-info", JSON.stringify(studentInfo));
				},
				console.error
			);
			break;
		case Role.Teacher:
			this.usersService.getTeacherInfo(this.loggedUser.username!).subscribe(
				teacherInfo => {
					this.teacherInfo = teacherInfo;
					localStorage.setItem("teacher-info", JSON.stringify(teacherInfo));
				},
				console.error
			);
			break;
		}
	}
}
