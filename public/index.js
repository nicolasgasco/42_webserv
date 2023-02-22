// Malforme request mock
const methodTooLongForm = document.getElementById("method-too-long-form");
const methodInput = document.getElementById("method-too-long-form__method");
const targetInput = document.getElementById("method-too-long-form__target");
const formMessage = document.querySelector(".method-too-long-form__message");

methodTooLongForm.addEventListener("submit", function (e) {
  e.preventDefault();

  fetch(targetInput.value, {
    method: methodInput.value,
  })
    .then(function (response) {
      // The API call was successful!
      console.log(response.status, response.statusText, !response.ok);
      if (!response.ok) {
        formMessage.classList.add("has-error");
      } else {
        formMessage.classList.remove("has-error");
      }
      formMessage.innerText = `${response.status} - ${response.statusText}`;

      return response.json();
    })
    .then(function (data) {
      // This is the JSON from our response
      console.log(data);
    })
    .catch(function (err) {
      // There was an error
      console.warn("Something went wrong.", err);
    });
});

// Query params
const queryParamsForm = document.querySelector(".query-params__form");
const codeInput = document.getElementById("code");
const messageInput = document.getElementById("message");

queryParamsForm.addEventListener("change", function () {
  const queryParamsLink = document.getElementById("query-params__link");
  const newTarget =
    "/error/default.html?" +
    `code=${codeInput.value}&message=${messageInput.value}`;
  queryParamsLink.href = newTarget;
  queryParamsLink.innerText = newTarget;
});

// Heavy video
let heavyVideoButton = document.getElementById("heavy-video-button");
let heavyVideoContainer = document.getElementById("heavy-video");
heavyVideoButton.addEventListener("click", function () {
  if (heavyVideoContainer.innerHTML) {
    heavyVideoContainer.innerHTML = null;
  } else {
    heavyVideoContainer.innerHTML = `
      <video width="320" height="240" controls>
        <source src="assets/heavy_video.mp4" type="video/mp4" />
          Your browser does not support the video tag.
      </video>`;
  }
});
