import {NgModule} from "@angular/core";
import {RouterModule, Routes} from "@angular/router";
import {WelcomeComponent} from "./pages/welcome/welcome.component";
import {AccountComponent} from "./pages/account/account.component";

const routes: Routes = [
	{
		path: "",
		component: WelcomeComponent
	},
	{
		path: "account",
		component: AccountComponent
	}
];

@NgModule({
	imports: [RouterModule.forRoot(routes)],
	exports: [RouterModule]
})
export class AppRoutingModule {
}
