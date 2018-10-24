var log4js = require('./node_modules/log4js')
var Web3 = require('./node_modules/web3')
var fs = require("fs")
var color = require("colors")

var logger = log4js.getLogger();
logger.level = 'info';

if (typeof web3 != 'undefined') {
  console.log("web3 is undefined")
  web3 = new Web3.currentProvider();
} else {
  web3 = new Web3(new Web3.providers.HttpProvider('http://localhost:22222'))
}

var base = "0x90f8bf6a479f320ead074411a4b0e7944ea8c9c1";

var code = fs.readFileSync("./bin/PopCnt.bin");
var abi = JSON.parse(fs.readFileSync("./bin/PopCnt.abi"));
var sol_testcontract = new web3.eth.Contract(abi);
var send_opt = {from:base, gas : 4000000}

// this was the thing missing
var popcnt_testcontract = new web3.eth.Contract(abi);

function popcnttest(c) {
  var in1 = 1024;
  var in2 = 0xffffff;

  c.methods.popcnt32(in1).call().then(res => logger.info(res));
  c.methods.popcnt64(in2).call().then(res => logger.info(res));
  c.methods.clz32(in1).call().then(res => logger.info(res));
  c.methods.clz64(in2).call().then(res => logger.info(res));
  c.methods.ctz32(in1).call().then(res => logger.info(res));
  c.methods.ctz64(in2).call().then(res => logger.info(res));
}

function popcnt_deploy(con) {
  popcnt_testcontract.deploy({data:code}).send(send_opt).then(
    contract => {
      logger.info('contract mined.');
      con(contract)
      logger.info("contract finished.")
    }
  )
}

popcnt_deploy(popcnttest);
