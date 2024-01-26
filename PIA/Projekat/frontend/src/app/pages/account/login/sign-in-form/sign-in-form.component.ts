import {Component, EventEmitter, Output} from "@angular/core";
import {MatInputModule} from "@angular/material/input";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {FormsModule} from "@angular/forms";
import {Credentials} from "../../../../services/users.service";

@Component({
	selector: "app-sign-in-form",
	templateUrl: "./sign-in-form.component.html",
	styleUrls: ["./sign-in-form.component.scss"],
	imports: [
		MatInputModule,
		MatIconModule,
		MatButtonModule,
		FormsModule
	],
	standalone: true
})
export class SignInFormComponent {
	public credentials: Credentials = {
		username: "",
		password: ""
	};

	@Output()
	public readonly signIn = new EventEmitter<Credentials>();

	@Output()
	public readonly signInBackup = new EventEmitter<string>();
}
