const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Valor = new Schema({
    Temperatura: {type: Number},
    Humedad: {type: Number},    
    Fecha: {type: Date, default: Date.now }
});
module.exports =mongoose.model('valores', Valor);