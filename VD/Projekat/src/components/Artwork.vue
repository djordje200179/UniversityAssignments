<template>
	<div class="col">
		<div class="card">
			<img :src="artwork.thumbnail" class="card-img-top" :alt="artwork.title">

			<div class="card-body" id="body">
				<h5 class="card-title text-nowrap text-truncate">{{ artwork.title }}</h5>
				<h6 class="card-subtitle mb-2 text-muted text-nowrap text-truncate">{{ artwork.artist }}</h6>
			</div>

			<div class="card-footer">
				<small class="text-muted">
					{{ $t("estimatedPrice") }}: {{ artwork.price }}
				</small>
			</div>
		</div>
	</div>
</template>

<script>
export default {
	name: "Artwork",
	props: {
		artwork: Object
	}
}
</script>

<style scoped>
.card>img {
	height: 200px;
	object-fit: cover;
}

.card-body {
	background-color: #8688b0;
}
</style>