const Http = new XMLHttpRequest();

const methodTooLongForm = document.getElementById("method-too-long-form");

methodTooLongForm.addEventListener("submit", function (e) {
  e.preventDefault();

  Http.open("DELETEE", "/error");
  Http.send();
});
