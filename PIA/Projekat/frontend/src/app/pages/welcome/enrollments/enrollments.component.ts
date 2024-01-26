import {AfterViewInit, Component, OnInit, ViewChild} from "@angular/core";
import {StatsService} from "../../../services/stats.service";
import {MatTableDataSource, MatTableModule} from "@angular/material/table";
import {MatSort, MatSortModule} from "@angular/material/sort";
import {MatInputModule} from "@angular/material/input";
import {FormsModule} from "@angular/forms";

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
		MatInputModule,
		FormsModule
	],
	standalone: true
})
export class EnrollmentsComponent implements OnInit {
	public allData? : Row[];

	public displayedColumns: string[] = ["firstName", "lastName", "subject"];

	public searchedSubject = "";
	public searchedFirstName = "";
	public searchedLastName = "";

	public constructor(private readonly statsService: StatsService) {

	}

	@ViewChild(MatSort)
	public sort?: MatSort;

	public getTableData() : MatTableDataSource<Row> {
		const filteredData = this.allData?.filter(row => {
			return row.subject.toLowerCase().includes(this.searchedSubject.toLowerCase()) &&
				row.firstName.toLowerCase().includes(this.searchedFirstName.toLowerCase()) &&
				row.lastName.toLowerCase().includes(this.searchedLastName.toLowerCase());
		});

		const source = new MatTableDataSource(filteredData);
		source.sort = this.sort!;

		return source;
	}

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

				this.allData = tableData;
			},
			console.error
		);
	}
}
