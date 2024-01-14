import {Component} from "@angular/core";
import {SignInFormComponent} from "../../components/sign-in-form/sign-in-form.component";
import {SignUpFormComponent} from "../../components/sign-up-form/sign-up-form.component";
import {Credentials, StudentInfo, TeacherInfo, UsersService} from "../../services/users.service";

@Component({
	selector: "app-welcome",
	templateUrl: "./welcome.component.html",
	styleUrls: ["./welcome.component.scss"],
	standalone: true,
	imports: [
		SignInFormComponent,
		SignUpFormComponent
	]
})
export class WelcomeComponent {
	public constructor(private readonly usersService: UsersService) {

	}

	public onSignIn(cred: Credentials) {
		this.usersService.signIn(cred).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				console.log(res);
			},
			err => {
				alert(err);
			}
		);
	}

	public onSignUpStudent(studentInfo: StudentInfo) {
		this.usersService.signUpStudent(studentInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				// navigate
				console.log(res);
			},
			err => {
				alert(err);
			}
		);
	}

	public onSignUpTeacher(teacherInfo: TeacherInfo) {
		this.usersService.signUpTeacher(teacherInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				// navigate
				console.log(res);
			},
			err => {
				alert(err);
			}
		);
	}
}
