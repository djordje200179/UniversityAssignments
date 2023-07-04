<template>
	<div class="row my-2 rounded p-4">
		<div class="col-lg-4">
			<img class="mx-auto d-block rounded" :src="artist.thumbnail" :alt="artist.name">
		</div>
		
		<div class="col-lg-8">
			<h4>{{ artist.name }}</h4>
			<p>{{ artist.biography[$i18n.locale] }}</p>

			<button type="button" class="btn btn-light ms-auto" @click="generateWorksDocument">
				{{ $t("artistsWorks") }}
			</button>
		</div>
	</div>
</template>

<script>
import { jsPDF } from "jspdf";

export default {
	name: "Artist",
	props: {
		artist: Object
	},
	methods: {
		generateWorksDocument() {
			const doc = new jsPDF();
			
			this.artist.artworks.forEach((artwork, i) => doc.text(artwork, 10, (i + 1) * 10));

			doc.save(`${this.artist.name}.pdf`);
		}
	}
}
</script>

<style scoped>
.row {
	background-color: #8688b0;
}

.row img {
	max-width: 100%;
	max-height: 400px;
	object-fit: contain;
}
</style>