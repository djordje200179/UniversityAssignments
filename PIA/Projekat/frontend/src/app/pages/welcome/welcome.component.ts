import {Component} from "@angular/core";
import {SignInFormComponent} from "../../components/sign-in-form/sign-in-form.component";
import {Credentials, UsersService} from "../../services/users.service";

@Component({
	selector: "app-welcome",
	templateUrl: "./welcome.component.html",
	styleUrls: ["./welcome.component.scss"],
	standalone: true,
	imports: [
		SignInFormComponent,
	],
})
export default class WelcomeComponent {
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
}
