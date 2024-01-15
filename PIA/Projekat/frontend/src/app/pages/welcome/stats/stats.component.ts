import {Component, OnInit} from "@angular/core";
import {Counters, StatsService} from "../../../services/stats.service";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";

@Component({
	selector: "app-stats",
	templateUrl: "./stats.component.html",
	styleUrls: ["./stats.component.scss"],
	imports: [
		MatCardModule,
		MatDividerModule
	],
	standalone: true
})
export class StatsComponent implements OnInit {
	public stats?: Counters;

	public constructor(private readonly statsService: StatsService) {

	}

	public ngOnInit() {
		this.statsService.getCounters().subscribe(
			stats => this.stats = stats,
			console.error
		);
	}
}
