let count = localStorage.length || 1;

//inputSpending function
//gets the amount, purpose, and date of a transaction from given input boxes and stores it into local storage
function inputSpending(){
  if(count>1){
    if(document.getElementById('update_budget').value == ' '){
      window.localStorage.setItem(count,JSON.stringify({amount:document.getElementById('amt_spent').value,date:document.getElementById('date').value,total:parseInt(JSON.parse(localStorage.getItem(localStorage.length)).total) + parseInt(document.getElementById('amt_spent').value)}))
    }else{
      document.getElementById('number').innerHTML = ': ' + document.getElementById('update_budget').value
      window.localStorage.setItem(count,JSON.stringify({amount:document.getElementById('amt_spent').value,date:document.getElementById('date').value,budget:document.getElementById('update_budget').value,total:parseInt(JSON.parse(localStorage.getItem(localStorage.length)).total) + parseInt(document.getElementById('amt_spent').value)}))
    }
  }else{
    if(document.getElementById('update_budget').value == ' '){
      window.localStorage.setItem(count,JSON.stringify({amount:document.getElementById('amt_spent').value,date:document.getElementById('date').value,budget:JSON.parse(localStorage.getItem(localStorage.length)).budget,total:parseInt(document.getElementById('amt_spent').value)}))
    }else{
      document.getElementById('number').innerHTML = ': ' + document.getElementById('update_budget').value
      window.localStorage.setItem(count,JSON.stringify({amount:document.getElementById('amt_spent').value,date:document.getElementById('date').value,budget:document.getElementById('update_budget').value,total:parseInt(document.getElementById('amt_spent').value)}))
    }
  }
  count++
  
  updateStatus()
  document.getElementById('update_budget').value = ' '
  document.getElementById('amt_spent').value = ' '
  document.getElementById('date').value = ' '
  updateList()
}
//updateList function
function updateList(){
  document.getElementById('data').innerHTML = ''
  for (let i = 0; i < count; i++) {
        let row = document.createElement('tr')
        let amount = JSON.parse(localStorage.getItem(i+1)).amount
        let date = JSON.parse(localStorage.getItem(i+1)).date
        let data = document.createElement('td')
        data.innerHTML = amount
        let data2 = document.createElement('td')
        data2.innerHTML = date
        row.appendChild(data)
        row.appendChild(data2)
        document.getElementById('data').appendChild(row)
  }
}
//clearData function
function cleardata(){
    localStorage.clear()
    count = 1
    document.getElementById('number').innerHTML = ''
    document.getElementById('status').innerHTML = ''
    updateList()
}
function updateStatus(){
  if(parseInt(document.getElementById('number').innerHTML)-parseInt(JSON.parse(localStorage.getItem(localStorage.length)).total)<0){
    document.getElementById('status').innerHTML = ': Bad'
  }else if(parseInt(document.getElementById('number').innerHTML)-parseInt(JSON.parse(localStorage.getItem(localStorage.length)).total)<0.05*parseInt(document.getElementById('number').innerHTML)){
    document.getElementById('status').innerHTML = ': Almost'
  }else{
    document.getElementById('status').innerHTML = ': Good'
  }
}

function load(){
  if(count>1){
    updateList()
    document.getElementById('number').innerHTML = JSON.parse(localStorage.getItem(localStorage.length)).budget
    updateStatus()
  }
}
