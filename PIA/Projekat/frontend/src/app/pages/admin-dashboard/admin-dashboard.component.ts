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


@Component({
	selector: "app-admin-dashboard",
	standalone: true,
	imports: [CommonModule, TeacherRequestsComponent, GraphsComponent, TeachersListComponent, StudentsListComponent],
	templateUrl: "./admin-dashboard.component.html",
	styleUrls: ["./admin-dashboard.component.scss"]
})
export class AdminDashboardComponent implements OnInit {
	public loggedUser: UserInfo | null;

	public constructor(
		private readonly usersService: UsersService,
		private readonly router: Router,
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
}
