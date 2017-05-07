const calculator = require('./lib/calculator');

calculator.add(1, 77).then((value) => {
    const p = document.createElement('p');
    p.innerText = value;
    document.body.appendChild(p);
});
