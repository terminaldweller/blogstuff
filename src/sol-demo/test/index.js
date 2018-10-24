const fs = require('fs')
const net = require('net')
const Web3 = require('web3')
const process = require('process')
const color = require('colors')
const child_process = require('child_process')

const useLocalSolc = true

function invokeCompiler(input) {
    if (useLocalSolc) {
        var result = child_process.spawnSync('solc', ['--optimize', '--standard-json', '-'], {input: input, encoding: 'utf-8'})
        if (result.status != 0) {
            console.log("Error invoking compiler:".red)
            console.log(result.output[2])
        }
        return result.output[1]
    } else {
        return require('solc').compileStandard(input)
    }
}

function checkForErrors(results) {
    if (!results) {
        console.log("Error invoking compiler (results is invalid).")
        console.log(results)
        process.exit(1)
    }
    var anyError = false
    if (results.errors) {
        for (var e of results.errors) {
            if (e.severity != 'warning') {
                console.log(e.formattedMessage.red)
                anyError = true
            }
        }
    }
    if (anyError) {
      console.log("compiler error".red)
        process.exit(1)
    }
}

function compile() {
    const solc = require('solc')
    function readFile(name) {
        return fs.readFileSync(name, {encoding: 'utf-8'})
    }

    const compilerInput = {
        'language': 'Solidity',
        'sources': {
            'popcnt.sol': {'content': readFile('contracts/popcnt.sol')}
        }
    }

    var res = invokeCompiler(JSON.stringify(compilerInput))
    console.log(res.green)
    var results = JSON.parse(invokeCompiler(JSON.stringify(compilerInput)))
    checkForErrors(results)
    Code = '0x' + results['contracts']['popcnt.sol']['PopCnt']['evm']['bytecode']['object']
    console.log(Code.blue)
    ABI = results['contracts']['popcnt.sol']['PopCnt']['abi']
    console.log(("ABI is", ABI).blue)
}

console.log("Compiling contracts...".green)
compile()

var ipcpath = '/tmp/ethereum_dev_mode/geth.ipc'
if (process.argv.length >= 3)
{
    ipcpath = process.argv[process.argv.length - 1]
}

var web3 = new Web3(new Web3.providers.IpcProvider(ipcpath, net))
var contractAddr = 0

async function setupAccount(_account) {
    console.log("Account setup...".green)
    var account = _account ? _account : await web3.eth.personal.newAccount('')
    console.log("Using account: ".cyan + account.cyan)
    await web3.eth.personal.unlockAccount(account, '', 1000000)
    return account
}

async function deployContract(c_code, c_abi, c_addr, b_account, c_gas, bool_log) {
  var block = await web3.eth.getBlockNumber()
    if (bool_log) console.log("At block " + block)
    var contract
    if (c_addr) {
        contract = new web3.eth.Contract(c_abi, c_addr)
    } else {
        contract = await new web3.eth.Contract(c_abi).deploy({data: c_code}).send({
            from: b_account,
            gas: c_gas
        })
    }
    if (bool_log) console.log("contract deployed at ".blue + contract.options.address.blue)
    return contract
}

async function test() {
  var account = await setupAccount()
  var c = await deployContract(Code, ABI, contractAddr, account, 4000000, true)
  var in1 = 1024;
  var in2 = 0xffffff;

  var ret = await c.methods.popcnt32(in1).call({from:account})
  console.log(("popcnt32 returned ", ret).red)
  ret = await c.methods.popcnt64(in2).call({from:account})
  console.log(ret.red)
  ret = await c.methods.clz32(in1).call({from:account})
  console.log(ret.red)
  ret = await c.methods.clz64(in2).call({from:account})
  console.log(ret.red)
  ret = await c.methods.ctz32(in1).call({from:account})
  console.log(ret.red)
  ret = await c.methods.ctz64(in2).call({from:account})
  console.log(ret.red)
}

var anyError = false
test()
