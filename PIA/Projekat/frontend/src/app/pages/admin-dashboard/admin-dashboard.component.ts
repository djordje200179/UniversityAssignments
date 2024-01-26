import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {Role, UserInfo, UsersService} from "../../services/users.service";
import {Router} from "@angular/router";
import {StatsService} from "../../services/stats.service";
import {CanvasJSAngularChartsModule} from "@canvasjs/angular-charts";
import {TeacherRequestsComponent} from "./teacher-requests/teacher-requests.component";
import {GraphsComponent} from "./graphs/graphs.component";
import {TeachersListComponent} from "./teachers-list/teachers-list.component";
import {StudentsListComponent} from "./students-list/students-list.component";
import {MatButtonModule} from "@angular/material/button";
import {TeachersService} from "../../services/teachers.service";
import {HttpErrorResponse} from "@angular/common/http";


@Component({
	selector: "app-admin-dashboard",
	standalone: true,
	imports: [CommonModule, TeacherRequestsComponent, GraphsComponent, TeachersListComponent, StudentsListComponent, MatButtonModule],
	templateUrl: "./admin-dashboard.component.html",
	styleUrls: ["./admin-dashboard.component.scss"]
})
export class AdminDashboardComponent implements OnInit {
	public loggedUser: UserInfo | null;

	public constructor(
		private readonly usersService: UsersService,
		private readonly router: Router,
		private readonly teachersService: TeachersService
	) {
		this.loggedUser = usersService.getCurrentUser();
	}

	public ngOnInit(): void {
		if (!this.loggedUser) {
			this.usersService.signInAdmin().subscribe(
				res => {
					localStorage.setItem("user-info", JSON.stringify(res));
					this.loggedUser = res;
					window.location.reload();
				},
				err => {
					console.error(err);
					this.router.navigate(["/"]);
				}
			);

			return;
		}

		if (this.loggedUser.role != Role.Admin) {
			this.router.navigate(["/"]);
			return;
		}
	}

	public addSubject(): void {
		const subjectName = prompt("Име предмета:");
		if (!subjectName)
			return;

		this.teachersService.addSubject(subjectName).subscribe(
			res => {
				alert("Предмет је успјешно додан!");
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.error(err);
					return;
				}

				if (err.status == 409) {
					alert("Предмет са тим именом већ постоји!");
					return;
				}

				alert("Непозната грешка!");
				console.error(err);
			}
		);
	}
}
