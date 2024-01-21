import {AfterViewInit, Component, OnInit, ViewChild} from "@angular/core";
import {StatsService} from "../../../services/stats.service";
import {MatTableDataSource, MatTableModule} from "@angular/material/table";
import {MatSort, MatSortModule} from "@angular/material/sort";
import {MatInputModule} from "@angular/material/input";

interface Row {
	firstName: string;
	lastName: string;
	subject: string;
}

@Component({
	selector: "app-enrollments",
	templateUrl: "./enrollments.component.html",
	styleUrls: ["./enrollments.component.scss"],
	imports: [
		MatTableModule,
		MatSortModule,
		MatInputModule
	],
	standalone: true
})
export class EnrollmentsComponent implements OnInit {
	public tableData?: MatTableDataSource<Row>;

	public displayedColumns: string[] = ["firstName", "lastName", "subject"];

	public constructor(private readonly statsService: StatsService) {

	}

	@ViewChild(MatSort)
	public sort?: MatSort;

	public ngOnInit() {
		this.statsService.getEnrollments().subscribe(
			enrollments => {
				const tableData: Row[] = [];
				for (const teacher in enrollments) {
					const [firstName, lastName] = teacher.split(" ");
					for (const subject of enrollments[teacher]) {
						tableData.push({
							firstName: firstName,
							lastName: lastName,
							subject: subject
						});
					}
				}

				const source = new MatTableDataSource(tableData);
				source.sort = this.sort!;

				this.tableData = source;
			},
			console.error
		);
	}
}
