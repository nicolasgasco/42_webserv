const urlSearchParams = new URLSearchParams(window.location.search);
const params = Object.fromEntries(urlSearchParams.entries());

const body = document.querySelector("body");

const re = new RegExp("{{(.*)}}", "g");

for (let [key, value] of Object.entries(params)) {
  const re = new RegExp(`{{${key}}}`, "g");

  if (re.test(body.innerText)) {
    body.innerHTML = body.innerHTML.replaceAll(re, value);
  }
}
