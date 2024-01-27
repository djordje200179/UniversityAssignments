import {NgModule} from "@angular/core";
import {BrowserModule} from "@angular/platform-browser";
import {AppRoutingModule} from "./app-routing.module";
import {AppComponent} from "./app.component";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";
import {HeaderComponent} from "./components/header/header.component";
import {HttpClientModule} from "@angular/common/http";
import {CalendarModule, DateAdapter} from "angular-calendar";
import {adapterFactory} from "angular-calendar/date-adapters/date-fns";
import {registerLocaleData} from "@angular/common";
import localeSr from '@angular/common/locales/sr-Cyrl-BA';
import {MAT_DIALOG_DEFAULT_OPTIONS, MatDialogModule} from "@angular/material/dialog";

registerLocaleData(localeSr);

@NgModule({
	declarations: [
		AppComponent,
	],
	imports: [
		BrowserModule,
		AppRoutingModule,
		BrowserAnimationsModule,
		HttpClientModule,
		MatDialogModule,
		HeaderComponent,
		CalendarModule.forRoot({provide: DateAdapter, useFactory: adapterFactory})
	],
	providers: [
		{provide: MAT_DIALOG_DEFAULT_OPTIONS, useValue: {hasBackdrop: true}}
	],
	exports: [],
	bootstrap: [AppComponent]
})
export class AppModule {
}
