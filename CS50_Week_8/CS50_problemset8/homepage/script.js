// JavaScript for Homepage Website

// Sets navigation bar on a page.
function setNav() {
    const navBar = `
    <nav>
        <ul>
            <li><a href="index.html">Home</a></li>
            <li><a href="rockclimbing.html">Rock Climbing</a></li>
            <li><a href="muaythai.html">Muay Thai</a></li>
            <li><a href="career.html">Career</a></li>
        </ul>
    </nav>
    `;

    document.querySelector("header").innerHTML += navBar;
    document.querySelector("footer").innerHTML += navBar;
}

