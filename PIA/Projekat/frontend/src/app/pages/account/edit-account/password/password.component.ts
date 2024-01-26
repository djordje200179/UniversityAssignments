import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatButtonModule} from "@angular/material/button";
import {MatCardModule} from "@angular/material/card";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatIconModule} from "@angular/material/icon";
import {MatInputModule} from "@angular/material/input";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";
import {MatButtonToggleModule} from "@angular/material/button-toggle";
import {AuthType, UserInfo, UsersService} from "../../../../services/users.service";
import {HttpErrorResponse} from "@angular/common/http";



@Component({
	selector: "app-password",
	standalone: true,
	imports: [CommonModule, MatButtonModule, MatCardModule, MatFormFieldModule, MatIconModule, MatInputModule, ReactiveFormsModule, MatButtonToggleModule, FormsModule],
	templateUrl: "./password.component.html",
	styleUrls: ["./password.component.scss"]
})
export class PasswordComponent {
	public authType: AuthType = AuthType.OldPassword;

	public securityAnswer: string = "";
	public oldPassword: string = "";
	public newPassword: string = "";
	public confirmedNewPassword: string = "";

	public userInfo: UserInfo;

	public constructor(private readonly usersService: UsersService) {
		this.userInfo = usersService.getCurrentUser()!;
	}

	public changePassword(): void {
		if (this.newPassword !== this.confirmedNewPassword) {
			alert("Лозинке се не поклапају!");
			return;
		}

		const passwordError = this.usersService.isPasswordValid(this.newPassword);
		if (passwordError)
			return alert(passwordError);

		const authAnswer = this.authType === AuthType.SecurityQuestion ? this.securityAnswer : this.oldPassword;

		this.usersService.changePassword(
			this.userInfo.username!, this.newPassword, authAnswer, this.authType
		).subscribe(
			res => {
				alert("Успјешно сте промијенили лозинку!");
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.error(err);
					return;
				}

				if (err.status === 400) {
					alert("Корисника није могуће аутентификовати!");
					return;
				}

				alert("Непозната грешка!");
				console.error(err);
				return;
			}
		);
	}
}
