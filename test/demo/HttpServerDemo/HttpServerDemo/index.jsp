<%@ page contentType="text/html;charset=GBK" language="java"%>

<html>
<head>
<title>Error</title>
<script>
	function displayDate() {
		document.getElementById("demo").innerHTML = Date();
	}
</script>
<script type="text/javascript" src="scripts/test.js"></script>
</head>
<body>
	<h3>жпнд╡Бйт</h3>

	<button type="button" onclick="displayDate()">Display Date</button>
	<p id="demo">This is a paragraph.</p>
	
	<button type="button" onclick="showTest()">Show Test</button>
</body>
</html>