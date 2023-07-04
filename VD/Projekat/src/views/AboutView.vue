<template>
	<div class="container mt-4">
		<div class="row text-white">
			<div class="col-lg-6">
				<p>{{ $t("aboutUsText1") }}</p>
				<p>{{ $t("aboutUsText2") }}</p>
				<p>{{ $t("aboutUsText3") }}</p>
				<p>{{ $t("aboutUsText4") }}</p>
				<p>{{ $t("aboutUsText5") }}</p>
				<p>{{ $t("aboutUsText6") }}</p>
			</div>

			<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d2562.4595055153127!2d20.468064035469425!3d44.76170768543742!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x475a713c95c5e01b%3A0xc42248b4768e7d38!2z0JHQtdC30LHQtdC00L3QvtGB0L3QviDQuNC90YTQvtGA0LzQsNGC0LjQstC90LAg0LDQs9C10L3RhtC40ZjQsCAtINCR0JjQkA!5e1!3m2!1ssr!2srs!4v1688418520168!5m2!1ssr!2srs"
				class="col-lg-6" width="600" height="450" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
		</div>
	</div>
</template>

<script>
export default {
	name: "AboutView",
	components: {
  
	}
}
</script>

<style scoped>
p {
	text-indent: 50px;
}
</style>