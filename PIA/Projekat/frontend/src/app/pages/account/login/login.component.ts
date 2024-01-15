import {Component} from "@angular/core";
import {Credentials, StudentInfo, TeacherInfo, UsersService} from "../../../services/users.service";
import {SignInFormComponent} from "./sign-in-form/sign-in-form.component";
import {SignUpFormComponent} from "./sign-up-form/sign-up-form.component";

@Component({
	selector: "app-login",
	templateUrl: "./login.component.html",
	styleUrls: ["./login.component.scss"],
	imports: [
		SignInFormComponent,
		SignUpFormComponent
	],
	standalone: true
})
export class LoginComponent {
	public constructor(private readonly usersService: UsersService) {

	}

	public onSignIn(cred: Credentials) {
		this.usersService.signIn(cred).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			console.error
		);
	}

	public onSignUpStudent(studentInfo: StudentInfo) {
		this.usersService.signUpStudent(studentInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			console.error
		);
	}

	public onSignUpTeacher(teacherInfo: TeacherInfo) {
		this.usersService.signUpTeacher(teacherInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			console.error
		);
	}
}
