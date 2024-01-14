import {Component} from "@angular/core";
import {SignInFormComponent} from "./sign-in-form/sign-in-form.component";
import {SignUpFormComponent} from "./sign-up-form/sign-up-form.component";
import {Credentials, StudentInfo, TeacherInfo, UsersService} from "../../services/users.service";
import {Router} from "@angular/router";

@Component({
	selector: "app-account",
	templateUrl: "./account.component.html",
	styleUrls: ["./account.component.scss"],
	imports: [
		SignInFormComponent,
		SignUpFormComponent
	],
	standalone: true
})
export class AccountComponent {
	public constructor(private readonly usersService: UsersService, private readonly router: Router) {

	}

	public onSignIn(cred: Credentials) {
		this.usersService.signIn(cred).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				this.router.navigate(["/"]);
			},
			alert
		);
	}

	public onSignUpStudent(studentInfo: StudentInfo) {
		this.usersService.signUpStudent(studentInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				this.router.navigate(["/"]);
			},
			alert
		);
	}

	public onSignUpTeacher(teacherInfo: TeacherInfo) {
		this.usersService.signUpTeacher(teacherInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				this.router.navigate(["/"]);
			},
			alert
		);
	}
}
