const Http = new XMLHttpRequest();

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

const queryParamsForm = document.querySelector(".query-params__form");
queryParamsForm.addEventListener("change", function () {
  let childrenValues = [];
  for (child of queryParamsForm.children) {
    childrenValues.push(child.value);
  }

  const queryParamsLink = document.getElementById("query-params__link");
  const newTarget =
    "/error/index.html?" +
    `${childrenValues[0] || "key"}=${childrenValues[1] || "value"}&${
      childrenValues[2] || "key"
    }=${childrenValues[3] || "value"}`;
  queryParamsLink.href = newTarget;
  queryParamsLink.innerText = newTarget;
});
