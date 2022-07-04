

function TestDefine(a){
    if ((typeof a) === "undefined")
        return; 
    console.log(a);
}

console.log(typeof xxx)
TestDefine(xxx);            //ReferenceError: xxx is not defined