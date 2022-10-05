animate_website();
handle_contact_form();


/**
 * Run animations for the entire website
 */
function animate_website()
{
  // Animate navbar (when page scrolled more than title section height)
  ScrollTrigger.create({
    trigger: ["#title", "#hero"],
    start: 'bottom 80',
    end: 99999,
    toggleClass: {className: 'header--scrolled', targets: 'header'}
  });

  // Animate title on load
  gsap.fromTo(".title-content", {y: 60, duration: 0.5}, {y: 0, opacity: 1});

  // Animate album cards on scroll (ref: https://codepen.io/GreenSock/pen/dyGyopR/823312ec3785be7b25315ec2efd517d8)
  ScrollTrigger.batch(".card", {
    interval: 0.1,
    batchMax: 3,
    onEnter: batch => gsap.to(batch, {autoAlpha: 1, stagger: 0.15, overwrite: true})
  });
}

/**
 * Handle the submission of a contact form
 */
function handle_contact_form()
{
  const submitBtn = document.querySelector("form input[type='submit']");
  submitBtn.addEventListener('click', e => {
    e.preventDefault();
    validate_fields(submitBtn.parentNode);
  });
}

/**
 * Check the validity of a form
 * 
 * @param {HTMLElement} formDiv The <form> element
 */
function validate_fields(formDiv) 
  {
    const email = formDiv.querySelector("input[id='email']");
    const message = formDiv.querySelector("textarea[id='message']");
    if (
      email.value.length > 0 
      && message.value.length > 0 
      // email validation regex ref: https://www.abstractapi.com/guides/email-address-pattern-validation
      && email.value.match(/^([-!#-'*+/-9=?A-Z^-~]+(\.[-!#-'*+/-9=?A-Z^-~]+)*|"([]!#-[^-~ \t]|(\\[\t -~]))+")@([0-9A-Za-z]([0-9A-Za-z-]{0,61}[0-9A-Za-z])?(\.[0-9A-Za-z]([0-9A-Za-z-]{0,61}[0-9A-Za-z])?)*|\[((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])){3}|IPv6:((((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){6}|::((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){5}|[0-9A-Fa-f]{0,4}::((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){4}|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):)?(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){3}|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){0,2}(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){2}|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){0,3}(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){0,4}(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::)((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3})|(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])){3})|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){0,5}(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3})|(((0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}):){0,6}(0|[1-9A-Fa-f][0-9A-Fa-f]{0,3}))?::)|(?!IPv6:)[0-9A-Za-z-]*[0-9A-Za-z]:[!-Z^-~]+)])$/)
    ) {
      show_alert("success", formDiv);
    } else {
      show_alert("danger", formDiv);
    }
  }

/**
 * Display an alert in a div
 * 
 * @param {String} alertClass Must be "success" or "danger"
 * @param {HTMLElement} div The HTML container to display the alert in
 */
function show_alert(alertClass, div) 
{
  const alertDiv = div.querySelector(".contact-alert");
  alertDiv.style.display = "block";
  alertDiv.classList.add("alert-" + alertClass);
  if (alertClass == "success") {
    // success message
    if (alertDiv.classList.contains("alert-danger")) {
      alertDiv.classList.remove("alert-danger")
    }
    alertDiv.style.display = "block";
    alertDiv.innerHTML = "<b>Thank you!</b> Your message has been send successfully. I will answer in a timely manner.";
  } else {
    // error message
    if (alertDiv.classList.contains("alert-success")) {
      alertDiv.classList.remove("alert-success")
    }
    alertDiv.innerHTML = "<b>The submited form is not valid!</b> All the fields are mandatory and the email address must be like john.doe@gmail.com";
  }
}
