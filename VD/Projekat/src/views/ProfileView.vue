<template>
	<h1 class="display-6 mt-3 mb-1 text-white text-center">
		{{ $t("submittedOffers") }}
	</h1>
	<table class="table table-dark mx-auto text-center shrinked-element">
		<thead>
			<tr>
				<th scope="col">{{ $t("artwork") }}</th>
				<th scope="col">{{ $t("amount") }}</th>
			</tr>
		</thead>
		<tbody>
			<tr v-for="bid in bids" :key="bid.id">
				<td>{{ bid.artwork }}</td>
				<td>{{ bid.offer }}</td>
			</tr>
		</tbody>
	</table>
	
	<h1 class="display-6 mt-3 mb-1 text-white text-center">
		{{ $t("comments") }}
	</h1>
	<table class="table table-dark mx-auto text-center shrinked-element">
		<thead>
			<tr>
				<th scope="col">{{ $t("artwork") }}</th>
				<th scope="col">{{ $t("comment") }}</th>
			</tr>
		</thead>
		<tbody>
			<tr v-for="comment in comments" :key="comment.id">
				<td>{{ comment.artwork }}</td>
				<td>{{ comment.comment }}</td>
			</tr>
		</tbody>
	</table>
</template>

<script>
export default {
	name: "ProfileView",
	computed: {
		comments() {
			return JSON.parse(localStorage.getItem("comments") || "[]");
		},
		bids() {
			return JSON.parse(localStorage.getItem("bids") || "[]");
		},
	}
}
</script>

<style scoped>
.shrinked-element {
	min-width: 400px;
	max-width: 60%;
}
</style>