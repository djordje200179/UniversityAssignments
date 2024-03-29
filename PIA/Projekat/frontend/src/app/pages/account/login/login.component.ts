import {Component} from "@angular/core";
import {Credentials, StudentInfo, TeacherInfo, UsersService} from "../../../services/users.service";
import {SignInFormComponent} from "./sign-in-form/sign-in-form.component";
import {SignUpFormComponent} from "./sign-up-form/sign-up-form.component";
import {HttpErrorResponse} from "@angular/common/http";

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

	private signInErrorHandler(err: any) {
		if (!(err instanceof HttpErrorResponse)) {
			alert("Непозната грешка!");
			console.error(err);
			return;
		}

		if (err.status === 404) {
			alert("Корисник не постоји!");
			return;
		}

		alert("Непозната грешка!");
		console.error(err);
	}

	public onSignIn(cred: Credentials) {
		this.usersService.signIn(cred).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			this.signInErrorHandler
		);
	}

	public onSignInBackup(username: string) {
		if (username === "") {
			alert("Морате унијети корисничко име!");
			return;
		}

		this.usersService.getSecurityQuestion(username).subscribe(
			question => {
				const answer = prompt(question);
				if (!answer)
					return;

				const credentials = {
					username: username,
					password: answer
				};

				this.usersService.signInBackup(credentials).subscribe(
					res => {
						localStorage.setItem("user-info", JSON.stringify(res));
						window.location.reload();
					},
					this.signInErrorHandler
				);
			},
			this.signInErrorHandler
		);
	}

	public onSignUpStudent(studentInfo: StudentInfo) {
		this.usersService.signUpStudent(studentInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.error(err);
					return;
				}

				if (err.status === 409) {
					alert("Корисник са тим подацима већ постоји!");
					return;
				}

				alert("Непозната грешка!");
				console.error(err);
			}
		);
	}

	public onSignUpTeacher(teacherInfo: TeacherInfo) {
		this.usersService.signUpTeacher(teacherInfo).subscribe(
			res => {
				localStorage.setItem("user-info", JSON.stringify(res));
				window.location.reload();
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.error(err);
					return;
				}

				if (err.status === 409) {
					alert("Корисник са тим подацима већ постоји!");
					return;
				}

				alert("Непозната грешка!");
				console.error(err);
			}
		);
	}
}
