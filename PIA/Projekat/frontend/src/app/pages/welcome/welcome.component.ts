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
		this.usersService.signIn(cred).subscribe(res => {
			if (!res) {
				alert("Invalid credentials");
				return;
			}

			// navigate
			console.log(res);
		});
	}

	public onSignUpStudent(studentInfo: StudentInfo) {
		this.usersService.signUpStudent(studentInfo).subscribe(res => {
			// navigate
			console.log(res);
		});
	}

	public onSignUpTeacher(teacherInfo: TeacherInfo) {
		this.usersService.signUpTeacher(teacherInfo).subscribe(res => {
			// navigate
			console.log(res);
		});
	}
}
