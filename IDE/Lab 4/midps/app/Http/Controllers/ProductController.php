<?php

namespace App\Http\Controllers;

use App\Product;

use App\Http\Requests;
use App\Http\Controllers\Controller;
use Request;

use Illuminate\Support\Facades\Storage;
use Illuminate\Support\Facades\File;

class ProductController extends Controller
{

    public function index(){
        $products = Product::all();
        return view('admin.products',['products' => $products]);
    }

    public function destroy($id){
        Product::destroy($id);
        return redirect('/admin/products');
    }

    public function newProduct(){
        return view('admin.new');
    }

    public function add() {

        $file = Request::file('file');


        $entry = new \App\File();


        $entry->save();

        $product  = new Product();
        $product->file_id=$entry->id;
        $product->name =Request::input('name');
        $product->description =Request::input('description');
        $product->price =Request::input('price');
        $product->imageurl =Request::input('imageurl');

        $product->save();

        return redirect('/admin/products');

    }
}